static void vsock_insert_unbound(struct vsock_sock *vsk)
{
	spin_lock_bh(&vsock_table_lock);
	__vsock_insert_bound(vsock_unbound_sockets, vsk);
	spin_unlock_bh(&vsock_table_lock);
}
