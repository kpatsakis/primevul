void rose_disconnect(struct sock *sk, int reason, int cause, int diagnostic)
{
	struct rose_sock *rose = rose_sk(sk);

	rose_stop_timer(sk);
	rose_stop_idletimer(sk);

	rose_clear_queues(sk);

	rose->lci   = 0;
	rose->state = ROSE_STATE_0;

	if (cause != -1)
		rose->cause = cause;

	if (diagnostic != -1)
		rose->diagnostic = diagnostic;

	sk->sk_state     = TCP_CLOSE;
	sk->sk_err       = reason;
	sk->sk_shutdown |= SEND_SHUTDOWN;

	if (!sock_flag(sk, SOCK_DEAD)) {
		sk->sk_state_change(sk);
		sock_set_flag(sk, SOCK_DEAD);
	}
}
