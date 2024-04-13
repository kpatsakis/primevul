static struct Packet *ssh_new_packet(void)
{
    struct Packet *pkt = snew(struct Packet);

    pkt->body = pkt->data = NULL;
    pkt->maxlen = 0;

    return pkt;
}
