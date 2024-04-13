static void __vsock_insert_connected(struct list_head *list,
				     struct vsock_sock *vsk)
{
	sock_hold(&vsk->sk);
	list_add(&vsk->connected_table, list);
}
