static int iucv_sock_release(struct socket *sock)
{
	struct sock *sk = sock->sk;
	int err = 0;

	if (!sk)
		return 0;

	iucv_sock_close(sk);

	sock_orphan(sk);
	iucv_sock_kill(sk);
	return err;
}
