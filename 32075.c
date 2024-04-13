static void ssh_process_incoming_data(Ssh ssh,
				      const unsigned char **data, int *datalen)
{
    struct Packet *pktin;

    pktin = ssh->s_rdpkt(ssh, data, datalen);
    if (pktin) {
	ssh->protocol(ssh, NULL, 0, pktin);
	ssh_free_packet(pktin);
    }
}
