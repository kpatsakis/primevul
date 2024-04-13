static struct Packet *ssh2_rdpkt(Ssh ssh, const unsigned char **data,
                                 int *datalen)
{
    struct rdpkt2_state_tag *st = &ssh->rdpkt2_state;

    crBegin(ssh->ssh2_rdpkt_crstate);

    st->pktin = ssh_new_packet();

    st->pktin->type = 0;
    st->pktin->length = 0;
    if (ssh->sccipher)
	st->cipherblk = ssh->sccipher->blksize;
    else
	st->cipherblk = 8;
    if (st->cipherblk < 8)
	st->cipherblk = 8;
    st->maclen = ssh->scmac ? ssh->scmac->len : 0;

    if (ssh->sccipher && (ssh->sccipher->flags & SSH_CIPHER_IS_CBC) &&
	ssh->scmac && !ssh->scmac_etm) {
	/*
	 * When dealing with a CBC-mode cipher, we want to avoid the
	 * possibility of an attacker's tweaking the ciphertext stream
	 * so as to cause us to feed the same block to the block
	 * cipher more than once and thus leak information
	 * (VU#958563).  The way we do this is not to take any
	 * decisions on the basis of anything we've decrypted until
	 * we've verified it with a MAC.  That includes the packet
	 * length, so we just read data and check the MAC repeatedly,
	 * and when the MAC passes, see if the length we've got is
	 * plausible.
         *
         * This defence is unnecessary in OpenSSH ETM mode, because
         * the whole point of ETM mode is that the attacker can't
         * tweak the ciphertext stream at all without the MAC
         * detecting it before we decrypt anything.
	 */

	/* May as well allocate the whole lot now. */
	st->pktin->data = snewn(OUR_V2_PACKETLIMIT + st->maclen + APIEXTRA,
				unsigned char);

	/* Read an amount corresponding to the MAC. */
	for (st->i = 0; st->i < st->maclen; st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}

	st->packetlen = 0;
	{
	    unsigned char seq[4];
	    ssh->scmac->start(ssh->sc_mac_ctx);
	    PUT_32BIT(seq, st->incoming_sequence);
	    ssh->scmac->bytes(ssh->sc_mac_ctx, seq, 4);
	}

	for (;;) { /* Once around this loop per cipher block. */
	    /* Read another cipher-block's worth, and tack it onto the end. */
	    for (st->i = 0; st->i < st->cipherblk; st->i++) {
		while ((*datalen) == 0)
		    crReturn(NULL);
		st->pktin->data[st->packetlen+st->maclen+st->i] = *(*data)++;
		(*datalen)--;
	    }
	    /* Decrypt one more block (a little further back in the stream). */
	    ssh->sccipher->decrypt(ssh->sc_cipher_ctx,
				   st->pktin->data + st->packetlen,
				   st->cipherblk);
	    /* Feed that block to the MAC. */
	    ssh->scmac->bytes(ssh->sc_mac_ctx,
			      st->pktin->data + st->packetlen, st->cipherblk);
	    st->packetlen += st->cipherblk;
	    /* See if that gives us a valid packet. */
	    if (ssh->scmac->verresult(ssh->sc_mac_ctx,
				      st->pktin->data + st->packetlen) &&
		((st->len = toint(GET_32BIT(st->pktin->data))) ==
                 st->packetlen-4))
		    break;
	    if (st->packetlen >= OUR_V2_PACKETLIMIT) {
		bombout(("No valid incoming packet found"));
		ssh_free_packet(st->pktin);
		crStop(NULL);
	    }	    
	}
	st->pktin->maxlen = st->packetlen + st->maclen;
	st->pktin->data = sresize(st->pktin->data,
				  st->pktin->maxlen + APIEXTRA,
				  unsigned char);
    } else if (ssh->scmac && ssh->scmac_etm) {
	st->pktin->data = snewn(4 + APIEXTRA, unsigned char);

        /*
         * OpenSSH encrypt-then-MAC mode: the packet length is
         * unencrypted, unless the cipher supports length encryption.
         */
	for (st->i = st->len = 0; st->i < 4; st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}
        /* Cipher supports length decryption, so do it */
        if (ssh->sccipher && (ssh->sccipher->flags & SSH_CIPHER_SEPARATE_LENGTH)) {
            /* Keep the packet the same though, so the MAC passes */
            unsigned char len[4];
            memcpy(len, st->pktin->data, 4);
            ssh->sccipher->decrypt_length(ssh->sc_cipher_ctx, len, 4, st->incoming_sequence);
            st->len = toint(GET_32BIT(len));
        } else {
            st->len = toint(GET_32BIT(st->pktin->data));
        }

	/*
	 * _Completely_ silly lengths should be stomped on before they
	 * do us any more damage.
	 */
	if (st->len < 0 || st->len > OUR_V2_PACKETLIMIT ||
	    st->len % st->cipherblk != 0) {
	    bombout(("Incoming packet length field was garbled"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}

	/*
	 * So now we can work out the total packet length.
	 */
	st->packetlen = st->len + 4;

	/*
	 * Allocate memory for the rest of the packet.
	 */
	st->pktin->maxlen = st->packetlen + st->maclen;
	st->pktin->data = sresize(st->pktin->data,
				  st->pktin->maxlen + APIEXTRA,
				  unsigned char);

	/*
	 * Read the remainder of the packet.
	 */
	for (st->i = 4; st->i < st->packetlen + st->maclen; st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}

	/*
	 * Check the MAC.
	 */
	if (ssh->scmac
	    && !ssh->scmac->verify(ssh->sc_mac_ctx, st->pktin->data,
				   st->len + 4, st->incoming_sequence)) {
	    bombout(("Incorrect MAC received on packet"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}

	/* Decrypt everything between the length field and the MAC. */
	if (ssh->sccipher)
	    ssh->sccipher->decrypt(ssh->sc_cipher_ctx,
				   st->pktin->data + 4,
				   st->packetlen - 4);
    } else {
	st->pktin->data = snewn(st->cipherblk + APIEXTRA, unsigned char);

	/*
	 * Acquire and decrypt the first block of the packet. This will
	 * contain the length and padding details.
	 */
	for (st->i = st->len = 0; st->i < st->cipherblk; st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}

	if (ssh->sccipher)
	    ssh->sccipher->decrypt(ssh->sc_cipher_ctx,
				   st->pktin->data, st->cipherblk);

	/*
	 * Now get the length figure.
	 */
	st->len = toint(GET_32BIT(st->pktin->data));

	/*
	 * _Completely_ silly lengths should be stomped on before they
	 * do us any more damage.
	 */
	if (st->len < 0 || st->len > OUR_V2_PACKETLIMIT ||
	    (st->len + 4) % st->cipherblk != 0) {
	    bombout(("Incoming packet was garbled on decryption"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}

	/*
	 * So now we can work out the total packet length.
	 */
	st->packetlen = st->len + 4;

	/*
	 * Allocate memory for the rest of the packet.
	 */
	st->pktin->maxlen = st->packetlen + st->maclen;
	st->pktin->data = sresize(st->pktin->data,
				  st->pktin->maxlen + APIEXTRA,
				  unsigned char);

	/*
	 * Read and decrypt the remainder of the packet.
	 */
	for (st->i = st->cipherblk; st->i < st->packetlen + st->maclen;
	     st->i++) {
	    while ((*datalen) == 0)
		crReturn(NULL);
	    st->pktin->data[st->i] = *(*data)++;
	    (*datalen)--;
	}
	/* Decrypt everything _except_ the MAC. */
	if (ssh->sccipher)
	    ssh->sccipher->decrypt(ssh->sc_cipher_ctx,
				   st->pktin->data + st->cipherblk,
				   st->packetlen - st->cipherblk);

	/*
	 * Check the MAC.
	 */
	if (ssh->scmac
	    && !ssh->scmac->verify(ssh->sc_mac_ctx, st->pktin->data,
				   st->len + 4, st->incoming_sequence)) {
	    bombout(("Incorrect MAC received on packet"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}
    }
    /* Get and sanity-check the amount of random padding. */
    st->pad = st->pktin->data[4];
    if (st->pad < 4 || st->len - st->pad < 1) {
	bombout(("Invalid padding length on received packet"));
	ssh_free_packet(st->pktin);
	crStop(NULL);
    }
    /*
     * This enables us to deduce the payload length.
     */
    st->payload = st->len - st->pad - 1;

    st->pktin->length = st->payload + 5;
    st->pktin->encrypted_len = st->packetlen;

    st->pktin->sequence = st->incoming_sequence++;

    st->pktin->length = st->packetlen - st->pad;
    assert(st->pktin->length >= 0);

    /*
     * Decompress packet payload.
     */
    {
	unsigned char *newpayload;
	int newlen;
	if (ssh->sccomp &&
	    ssh->sccomp->decompress(ssh->sc_comp_ctx,
				    st->pktin->data + 5, st->pktin->length - 5,
				    &newpayload, &newlen)) {
	    if (st->pktin->maxlen < newlen + 5) {
		st->pktin->maxlen = newlen + 5;
		st->pktin->data = sresize(st->pktin->data,
					  st->pktin->maxlen + APIEXTRA,
					  unsigned char);
	    }
	    st->pktin->length = 5 + newlen;
	    memcpy(st->pktin->data + 5, newpayload, newlen);
	    sfree(newpayload);
	}
    }

    /*
     * RFC 4253 doesn't explicitly say that completely empty packets
     * with no type byte are forbidden, so treat them as deserving
     * an SSH_MSG_UNIMPLEMENTED.
     */
    if (st->pktin->length <= 5) { /* == 5 we hope, but robustness */
        ssh2_msg_something_unimplemented(ssh, st->pktin);
        crStop(NULL);
    }
    /*
     * pktin->body and pktin->length should identify the semantic
     * content of the packet, excluding the initial type byte.
     */
    st->pktin->type = st->pktin->data[5];
    st->pktin->body = st->pktin->data + 6;
    st->pktin->length -= 6;
    assert(st->pktin->length >= 0);    /* one last double-check */

    if (ssh->logctx)
        ssh2_log_incoming_packet(ssh, st->pktin);

    st->pktin->savedpos = 0;

    crFinish(st->pktin);
}
