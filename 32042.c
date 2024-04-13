static int ssh2_pkt_getbool(struct Packet *pkt)
{
    unsigned long value;
    if (pkt->length - pkt->savedpos < 1)
	return 0;		       /* arrgh, no way to decline (FIXME?) */
    value = pkt->body[pkt->savedpos] != 0;
    pkt->savedpos++;
    return value;
}
