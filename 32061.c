static void ssh_free_packet(struct Packet *pkt)
{
    sfree(pkt->data);
    sfree(pkt);
}
