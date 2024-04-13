static long caif_stream_data_wait(struct sock *sk, long timeo)
{
	DEFINE_WAIT(wait);
	lock_sock(sk);

	for (;;) {
		prepare_to_wait(sk_sleep(sk), &wait, TASK_INTERRUPTIBLE);

		if (!skb_queue_empty(&sk->sk_receive_queue) ||
			sk->sk_err ||
			sk->sk_state != CAIF_CONNECTED ||
			sock_flag(sk, SOCK_DEAD) ||
			(sk->sk_shutdown & RCV_SHUTDOWN) ||
			signal_pending(current) ||
			!timeo)
			break;

		set_bit(SOCK_ASYNC_WAITDATA, &sk->sk_socket->flags);
		release_sock(sk);
		timeo = schedule_timeout(timeo);
		lock_sock(sk);
		clear_bit(SOCK_ASYNC_WAITDATA, &sk->sk_socket->flags);
	}

	finish_wait(sk_sleep(sk), &wait);
	release_sock(sk);
	return timeo;
}
