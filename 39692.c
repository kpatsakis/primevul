void vsock_add_pending(struct sock *listener, struct sock *pending)
{
	struct vsock_sock *vlistener;
	struct vsock_sock *vpending;

	vlistener = vsock_sk(listener);
	vpending = vsock_sk(pending);

	sock_hold(pending);
	sock_hold(listener);
	list_add_tail(&vpending->pending_links, &vlistener->pending_links);
}
