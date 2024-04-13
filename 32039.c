static int ssh2_pkt_construct(Ssh ssh, struct Packet *pkt)
{
    int cipherblk, maclen, padding, unencrypted_prefix, i;

    if (ssh->logctx)
        ssh2_log_outgoing_packet(ssh, pkt);

    if (ssh->bare_connection) {
        /*
         * Trivial packet construction for the bare connection
         * protocol.
         */
        PUT_32BIT(pkt->data + 1, pkt->length - 5);
        pkt->body = pkt->data + 1;
        ssh->v2_outgoing_sequence++;   /* only for diagnostics, really */
        return pkt->length - 1;
    }

    /*
     * Compress packet payload.
     */
    {
	unsigned char *newpayload;
	int newlen;
	if (ssh->cscomp &&
	    ssh->cscomp->compress(ssh->cs_comp_ctx, pkt->data + 5,
				  pkt->length - 5,
				  &newpayload, &newlen)) {
	    pkt->length = 5;
	    ssh2_pkt_adddata(pkt, newpayload, newlen);
	    sfree(newpayload);
	}
    }

    /*
     * Add padding. At least four bytes, and must also bring total
     * length (minus MAC) up to a multiple of the block size.
     * If pkt->forcepad is set, make sure the packet is at least that size
     * after padding.
     */
    cipherblk = ssh->cscipher ? ssh->cscipher->blksize : 8;  /* block size */
    cipherblk = cipherblk < 8 ? 8 : cipherblk;	/* or 8 if blksize < 8 */
    padding = 4;
    unencrypted_prefix = (ssh->csmac && ssh->csmac_etm) ? 4 : 0;
    if (pkt->length + padding < pkt->forcepad)
	padding = pkt->forcepad - pkt->length;
    padding +=
	(cipherblk - (pkt->length - unencrypted_prefix + padding) % cipherblk)
        % cipherblk;
    assert(padding <= 255);
    maclen = ssh->csmac ? ssh->csmac->len : 0;
    ssh2_pkt_ensure(pkt, pkt->length + padding + maclen);
    pkt->data[4] = padding;
    for (i = 0; i < padding; i++)
	pkt->data[pkt->length + i] = random_byte();
    PUT_32BIT(pkt->data, pkt->length + padding - 4);

    /* Encrypt length if the scheme requires it */
    if (ssh->cscipher && (ssh->cscipher->flags & SSH_CIPHER_SEPARATE_LENGTH)) {
        ssh->cscipher->encrypt_length(ssh->cs_cipher_ctx, pkt->data, 4,
                                      ssh->v2_outgoing_sequence);
    }

    if (ssh->csmac && ssh->csmac_etm) {
        /*
         * OpenSSH-defined encrypt-then-MAC protocol.
         */
        if (ssh->cscipher)
            ssh->cscipher->encrypt(ssh->cs_cipher_ctx,
                                   pkt->data + 4, pkt->length + padding - 4);
        ssh->csmac->generate(ssh->cs_mac_ctx, pkt->data,
                             pkt->length + padding,
                             ssh->v2_outgoing_sequence);
    } else {
        /*
         * SSH-2 standard protocol.
         */
        if (ssh->csmac)
            ssh->csmac->generate(ssh->cs_mac_ctx, pkt->data,
                                 pkt->length + padding,
                                 ssh->v2_outgoing_sequence);
        if (ssh->cscipher)
            ssh->cscipher->encrypt(ssh->cs_cipher_ctx,
                                   pkt->data, pkt->length + padding);
    }

    ssh->v2_outgoing_sequence++;       /* whether or not we MACed */
    pkt->encrypted_len = pkt->length + padding;

    /* Ready-to-send packet starts at pkt->data. We return length. */
    pkt->body = pkt->data;
    return pkt->length + padding + maclen;
}
