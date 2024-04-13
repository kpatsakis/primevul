static void inet_get_ping_group_range_net(struct net *net, kgid_t *low,
					  kgid_t *high)
{
	kgid_t *data = net->ipv4.sysctl_ping_group_range;
	unsigned int seq;

	do {
		seq = read_seqbegin(&net->ipv4.sysctl_local_ports.lock);

		*low = data[0];
		*high = data[1];
	} while (read_seqretry(&net->ipv4.sysctl_local_ports.lock, seq));
}
