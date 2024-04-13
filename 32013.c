static void defer_packet(Ssh ssh, int pkttype, ...)
{
    struct Packet *pkt;
    va_list ap;
    va_start(ap, pkttype);
    pkt = construct_packet(ssh, pkttype, ap);
    va_end(ap);
    s_wrpkt_defer(ssh, pkt);
}
