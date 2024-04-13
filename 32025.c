static int s_wrpkt_prepare(Ssh ssh, struct Packet *pkt, int *offset_p)
{
    int pad, biglen, i, pktoffs;
    unsigned long crc;
#ifdef __SC__
    /*
     * XXX various versions of SC (including 8.8.4) screw up the
     * register allocation in this function and use the same register
     * (D6) for len and as a temporary, with predictable results.  The
     * following sledgehammer prevents this.
     */
    volatile
#endif
    int len;

    if (ssh->logctx)
        ssh1_log_outgoing_packet(ssh, pkt);

    if (ssh->v1_compressing) {
	unsigned char *compblk;
	int complen;
	zlib_compress_block(ssh->cs_comp_ctx,
			    pkt->data + 12, pkt->length - 12,
			    &compblk, &complen);
	ssh_pkt_ensure(pkt, complen + 2);   /* just in case it's got bigger */
	memcpy(pkt->data + 12, compblk, complen);
	sfree(compblk);
	pkt->length = complen + 12;
    }

    ssh_pkt_ensure(pkt, pkt->length + 4); /* space for CRC */
    pkt->length += 4;
    len = pkt->length - 4 - 8;	/* len(type+data+CRC) */
    pad = 8 - (len % 8);
    pktoffs = 8 - pad;
    biglen = len + pad;		/* len(padding+type+data+CRC) */

    for (i = pktoffs; i < 4+8; i++)
	pkt->data[i] = random_byte();
    crc = crc32_compute(pkt->data + pktoffs + 4, biglen - 4); /* all ex len */
    PUT_32BIT(pkt->data + pktoffs + 4 + biglen - 4, crc);
    PUT_32BIT(pkt->data + pktoffs, len);

    if (ssh->cipher)
	ssh->cipher->encrypt(ssh->v1_cipher_ctx,
			     pkt->data + pktoffs + 4, biglen);

    if (offset_p) *offset_p = pktoffs;
    return biglen + 4;		/* len(length+padding+type+data+CRC) */
}
