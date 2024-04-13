void vsock_remove_bound(struct vsock_sock *vsk)
{
	spin_lock_bh(&vsock_table_lock);
	__vsock_remove_bound(vsk);
	spin_unlock_bh(&vsock_table_lock);
}
