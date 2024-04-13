void vsock_pending_work(struct work_struct *work)
{
	struct sock *sk;
	struct sock *listener;
	struct vsock_sock *vsk;
	bool cleanup;

	vsk = container_of(work, struct vsock_sock, dwork.work);
	sk = sk_vsock(vsk);
	listener = vsk->listener;
	cleanup = true;

	lock_sock(listener);
	lock_sock(sk);

	if (vsock_is_pending(sk)) {
		vsock_remove_pending(listener, sk);
	} else if (!vsk->rejected) {
		/* We are not on the pending list and accept() did not reject
		 * us, so we must have been accepted by our user process.  We
		 * just need to drop our references to the sockets and be on
		 * our way.
		 */
		cleanup = false;
		goto out;
	}

	listener->sk_ack_backlog--;

	/* We need to remove ourself from the global connected sockets list so
	 * incoming packets can't find this socket, and to reduce the reference
	 * count.
	 */
	if (vsock_in_connected_table(vsk))
		vsock_remove_connected(vsk);

	sk->sk_state = SS_FREE;

out:
	release_sock(sk);
	release_sock(listener);
	if (cleanup)
		sock_put(sk);

	sock_put(sk);
	sock_put(listener);
}
