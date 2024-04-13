static struct Packet *ssh1_rdpkt(Ssh ssh, const unsigned char **data,
                                 int *datalen)
{
    struct rdpkt1_state_tag *st = &ssh->rdpkt1_state;

    crBegin(ssh->ssh1_rdpkt_crstate);

    st->pktin = ssh_new_packet();

    st->pktin->type = 0;
    st->pktin->length = 0;

    for (st->i = st->len = 0; st->i < 4; st->i++) {
	while ((*datalen) == 0)
	    crReturn(NULL);
	st->len = (st->len << 8) + **data;
	(*data)++, (*datalen)--;
    }

    st->pad = 8 - (st->len % 8);
    st->biglen = st->len + st->pad;
    st->pktin->length = st->len - 5;

    if (st->biglen < 0) {
        bombout(("Extremely large packet length from server suggests"
		 " data stream corruption"));
	ssh_free_packet(st->pktin);
        crStop(NULL);
    }

    st->pktin->maxlen = st->biglen;
    st->pktin->data = snewn(st->biglen + APIEXTRA, unsigned char);

    st->to_read = st->biglen;
    st->p = st->pktin->data;
    while (st->to_read > 0) {
	st->chunk = st->to_read;
	while ((*datalen) == 0)
	    crReturn(NULL);
	if (st->chunk > (*datalen))
	    st->chunk = (*datalen);
	memcpy(st->p, *data, st->chunk);
	*data += st->chunk;
	*datalen -= st->chunk;
	st->p += st->chunk;
	st->to_read -= st->chunk;
    }

    if (ssh->cipher && detect_attack(ssh->crcda_ctx, st->pktin->data,
				     st->biglen, NULL)) {
        bombout(("Network attack (CRC compensation) detected!"));
	ssh_free_packet(st->pktin);
        crStop(NULL);
    }

    if (ssh->cipher)
	ssh->cipher->decrypt(ssh->v1_cipher_ctx, st->pktin->data, st->biglen);

    st->realcrc = crc32_compute(st->pktin->data, st->biglen - 4);
    st->gotcrc = GET_32BIT(st->pktin->data + st->biglen - 4);
    if (st->gotcrc != st->realcrc) {
	bombout(("Incorrect CRC received on packet"));
	ssh_free_packet(st->pktin);
	crStop(NULL);
    }

    st->pktin->body = st->pktin->data + st->pad + 1;

    if (ssh->v1_compressing) {
	unsigned char *decompblk;
	int decomplen;
	if (!zlib_decompress_block(ssh->sc_comp_ctx,
				   st->pktin->body - 1, st->pktin->length + 1,
				   &decompblk, &decomplen)) {
	    bombout(("Zlib decompression encountered invalid data"));
	    ssh_free_packet(st->pktin);
	    crStop(NULL);
	}

	if (st->pktin->maxlen < st->pad + decomplen) {
	    st->pktin->maxlen = st->pad + decomplen;
	    st->pktin->data = sresize(st->pktin->data,
				      st->pktin->maxlen + APIEXTRA,
				      unsigned char);
	    st->pktin->body = st->pktin->data + st->pad + 1;
	}

	memcpy(st->pktin->body - 1, decompblk, decomplen);
	sfree(decompblk);
	st->pktin->length = decomplen - 1;
    }

    st->pktin->type = st->pktin->body[-1];

    /*
     * Now pktin->body and pktin->length identify the semantic content
     * of the packet, excluding the initial type byte.
     */

    if (ssh->logctx)
        ssh1_log_incoming_packet(ssh, st->pktin);

    st->pktin->savedpos = 0;

    crFinish(st->pktin);
}
