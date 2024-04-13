static int iucv_sock_create(struct net *net, struct socket *sock, int protocol,
			    int kern)
{
	struct sock *sk;

	if (protocol && protocol != PF_IUCV)
		return -EPROTONOSUPPORT;

	sock->state = SS_UNCONNECTED;

	switch (sock->type) {
	case SOCK_STREAM:
		sock->ops = &iucv_sock_ops;
		break;
	case SOCK_SEQPACKET:
		/* currently, proto ops can handle both sk types */
		sock->ops = &iucv_sock_ops;
		break;
	default:
		return -ESOCKTNOSUPPORT;
	}

	sk = iucv_sock_alloc(sock, protocol, GFP_KERNEL);
	if (!sk)
		return -ENOMEM;

	iucv_sock_init(sk, NULL);

	return 0;
}
