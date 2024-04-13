static void __vsock_remove_connected(struct vsock_sock *vsk)
{
	list_del_init(&vsk->connected_table);
	sock_put(&vsk->sk);
}
