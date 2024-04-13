static void ssh2_pkt_defer(Ssh ssh, struct Packet *pkt)
{
    if (ssh->queueing)
	ssh2_pkt_queue(ssh, pkt);
    else
	ssh2_pkt_defer_noqueue(ssh, pkt, FALSE);
}
