static int l2tp_ip6_open(struct sock *sk)
{
	/* Prevent autobind. We don't have ports. */
	inet_sk(sk)->inet_num = IPPROTO_L2TP;

	write_lock_bh(&l2tp_ip6_lock);
	sk_add_node(sk, &l2tp_ip6_table);
	write_unlock_bh(&l2tp_ip6_lock);

	return 0;
}
