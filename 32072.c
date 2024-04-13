static unsigned long ssh_pkt_getuint32(struct Packet *pkt)
{
    unsigned long value;
    if (pkt->length - pkt->savedpos < 4)
	return 0;		       /* arrgh, no way to decline (FIXME?) */
    value = GET_32BIT(pkt->body + pkt->savedpos);
    pkt->savedpos += 4;
    return value;
}
