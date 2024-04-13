static void nr_kill_by_device(struct net_device *dev)
{
	struct sock *s;

	spin_lock_bh(&nr_list_lock);
	sk_for_each(s, &nr_list)
		if (nr_sk(s)->device == dev)
			nr_disconnect(s, ENETUNREACH);
	spin_unlock_bh(&nr_list_lock);
}
