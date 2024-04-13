void vsock_enqueue_accept(struct sock *listener, struct sock *connected)
{
	struct vsock_sock *vlistener;
	struct vsock_sock *vconnected;

	vlistener = vsock_sk(listener);
	vconnected = vsock_sk(connected);

	sock_hold(connected);
	sock_hold(listener);
	list_add_tail(&vconnected->accept_queue, &vlistener->accept_queue);
}
