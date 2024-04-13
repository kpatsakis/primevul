void vsock_remove_pending(struct sock *listener, struct sock *pending)
{
	struct vsock_sock *vpending = vsock_sk(pending);

	list_del_init(&vpending->pending_links);
	sock_put(listener);
	sock_put(pending);
}
