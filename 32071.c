static void ssh_pkt_getstring(struct Packet *pkt, char **p, int *length)
{
    int len;
    *p = NULL;
    *length = 0;
    if (pkt->length - pkt->savedpos < 4)
	return;
    len = toint(GET_32BIT(pkt->body + pkt->savedpos));
    if (len < 0)
	return;
    *length = len;
    pkt->savedpos += 4;
    if (pkt->length - pkt->savedpos < *length)
	return;
    *p = (char *)(pkt->body + pkt->savedpos);
    pkt->savedpos += *length;
}
