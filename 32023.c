static void s_wrpkt(Ssh ssh, struct Packet *pkt)
{
    int len, backlog, offset;
    len = s_wrpkt_prepare(ssh, pkt, &offset);
    backlog = s_write(ssh, pkt->data + offset, len);
    if (backlog > SSH_MAX_BACKLOG)
	ssh_throttle_all(ssh, 1, backlog);
    ssh_free_packet(pkt);
}
