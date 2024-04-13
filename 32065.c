static void ssh_pkt_addstring(struct Packet *pkt, const char *data)
{
    ssh_pkt_addstring_start(pkt);
    ssh_pkt_addstring_str(pkt, data);
}
