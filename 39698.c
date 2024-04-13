static struct sock *vsock_dequeue_accept(struct sock *listener)
{
	struct vsock_sock *vlistener;
	struct vsock_sock *vconnected;

	vlistener = vsock_sk(listener);

	if (list_empty(&vlistener->accept_queue))
		return NULL;

	vconnected = list_entry(vlistener->accept_queue.next,
				struct vsock_sock, accept_queue);

	list_del_init(&vconnected->accept_queue);
	sock_put(listener);
	/* The caller will need a reference on the connected socket so we let
	 * it call sock_put().
	 */

	return sk_vsock(vconnected);
}
