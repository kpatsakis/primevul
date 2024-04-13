void vsock_insert_connected(struct vsock_sock *vsk)
{
	struct list_head *list = vsock_connected_sockets(
		&vsk->remote_addr, &vsk->local_addr);

	spin_lock_bh(&vsock_table_lock);
	__vsock_insert_connected(list, vsk);
	spin_unlock_bh(&vsock_table_lock);
}
