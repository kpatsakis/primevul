static void ssh2_pkt_queuesend(Ssh ssh)
{
    int i;

    assert(!ssh->queueing);

    for (i = 0; i < ssh->queuelen; i++)
	ssh2_pkt_defer_noqueue(ssh, ssh->queue[i], FALSE);
    ssh->queuelen = 0;

    ssh_pkt_defersend(ssh);
}
