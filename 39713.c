static bool vsock_is_accept_queue_empty(struct sock *sk)
{
	struct vsock_sock *vsk = vsock_sk(sk);
	return list_empty(&vsk->accept_queue);
}
