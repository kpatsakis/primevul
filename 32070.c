static void ssh_pkt_ensure(struct Packet *pkt, int length)
{
    if (pkt->maxlen < length) {
	unsigned char *body = pkt->body;
	int offset = body ? body - pkt->data : 0;
	pkt->maxlen = length + 256;
	pkt->data = sresize(pkt->data, pkt->maxlen + APIEXTRA, unsigned char);
	if (body) pkt->body = pkt->data + offset;
    }
}
