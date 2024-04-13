static int vsock_dgram_recvmsg(struct kiocb *kiocb, struct socket *sock,
			       struct msghdr *msg, size_t len, int flags)
{
	return transport->dgram_dequeue(kiocb, vsock_sk(sock->sk), msg, len,
					flags);
}
