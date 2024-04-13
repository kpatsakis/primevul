static void ssh_pkt_addstring_start(struct Packet *pkt)
{
    ssh_pkt_adduint32(pkt, 0);
    pkt->savedpos = pkt->length;
}
