static void ssh_pkt_adduint32(struct Packet *pkt, unsigned long value)
{
    unsigned char x[4];
    PUT_32BIT(x, value);
    ssh_pkt_adddata(pkt, x, 4);
}
