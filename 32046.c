static void ssh2_pkt_send(Ssh ssh, struct Packet *pkt)
{
    if (ssh->queueing)
	ssh2_pkt_queue(ssh, pkt);
    else
	ssh2_pkt_send_noqueue(ssh, pkt);
}
