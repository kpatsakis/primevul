static void vsock_connect_timeout(struct work_struct *work)
{
	struct sock *sk;
	struct vsock_sock *vsk;

	vsk = container_of(work, struct vsock_sock, dwork.work);
	sk = sk_vsock(vsk);

	lock_sock(sk);
	if (sk->sk_state == SS_CONNECTING &&
	    (sk->sk_shutdown != SHUTDOWN_MASK)) {
		sk->sk_state = SS_UNCONNECTED;
		sk->sk_err = ETIMEDOUT;
		sk->sk_error_report(sk);
	}
	release_sock(sk);

	sock_put(sk);
}
