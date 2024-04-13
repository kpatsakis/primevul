static int vsock_create(struct net *net, struct socket *sock,
			int protocol, int kern)
{
	if (!sock)
		return -EINVAL;

	if (protocol && protocol != PF_VSOCK)
		return -EPROTONOSUPPORT;

	switch (sock->type) {
	case SOCK_DGRAM:
		sock->ops = &vsock_dgram_ops;
		break;
	case SOCK_STREAM:
		sock->ops = &vsock_stream_ops;
		break;
	default:
		return -ESOCKTNOSUPPORT;
	}

	sock->state = SS_UNCONNECTED;

	return __vsock_create(net, sock, NULL, GFP_KERNEL, 0) ? 0 : -ENOMEM;
}
