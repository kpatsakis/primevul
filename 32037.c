static void ssh2_pkt_addbool(struct Packet *pkt, unsigned char value)
{
    ssh_pkt_adddata(pkt, &value, 1);
}
