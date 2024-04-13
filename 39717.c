static int vsock_release(struct socket *sock)
{
	__vsock_release(sock->sk);
	sock->sk = NULL;
	sock->state = SS_FREE;

	return 0;
}
