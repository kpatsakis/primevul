static int iucv_sock_listen(struct socket *sock, int backlog)
{
	struct sock *sk = sock->sk;
	int err;

	lock_sock(sk);

	err = -EINVAL;
	if (sk->sk_state != IUCV_BOUND)
		goto done;

	if (sock->type != SOCK_STREAM && sock->type != SOCK_SEQPACKET)
		goto done;

	sk->sk_max_ack_backlog = backlog;
	sk->sk_ack_backlog = 0;
	sk->sk_state = IUCV_LISTEN;
	err = 0;

done:
	release_sock(sk);
	return err;
}
