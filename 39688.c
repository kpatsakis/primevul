static void __vsock_remove_bound(struct vsock_sock *vsk)
{
	list_del_init(&vsk->bound_table);
	sock_put(&vsk->sk);
}
