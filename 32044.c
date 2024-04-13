static void ssh2_pkt_queue(Ssh ssh, struct Packet *pkt)
{
    assert(ssh->queueing);

    if (ssh->queuelen >= ssh->queuesize) {
	ssh->queuesize = ssh->queuelen + 32;
	ssh->queue = sresize(ssh->queue, ssh->queuesize, struct Packet *);
    }

    ssh->queue[ssh->queuelen++] = pkt;
}
