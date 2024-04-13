static bool vsock_in_bound_table(struct vsock_sock *vsk)
{
	bool ret;

	spin_lock_bh(&vsock_table_lock);
	ret = __vsock_in_bound_table(vsk);
	spin_unlock_bh(&vsock_table_lock);

	return ret;
}
