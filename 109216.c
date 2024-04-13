static int ieee802154_create(struct net *net, struct socket *sock,
			     int protocol, int kern)
{
	struct sock *sk;
	int rc;
	struct proto *proto;
	const struct proto_ops *ops;

	if (!net_eq(net, &init_net))
		return -EAFNOSUPPORT;

	switch (sock->type) {
	case SOCK_RAW:
		proto = &ieee802154_raw_prot;
		ops = &ieee802154_raw_ops;
		break;
	case SOCK_DGRAM:
		proto = &ieee802154_dgram_prot;
		ops = &ieee802154_dgram_ops;
		break;
	default:
		rc = -ESOCKTNOSUPPORT;
		goto out;
	}

	rc = -ENOMEM;
	sk = sk_alloc(net, PF_IEEE802154, GFP_KERNEL, proto, kern);
	if (!sk)
		goto out;
	rc = 0;

	sock->ops = ops;

	sock_init_data(sock, sk);
	/* FIXME: sk->sk_destruct */
	sk->sk_family = PF_IEEE802154;

	/* Checksums on by default */
	sock_set_flag(sk, SOCK_ZAPPED);

	if (sk->sk_prot->hash) {
		rc = sk->sk_prot->hash(sk);
		if (rc) {
			sk_common_release(sk);
			goto out;
		}
	}

	if (sk->sk_prot->init) {
		rc = sk->sk_prot->init(sk);
		if (rc)
			sk_common_release(sk);
	}
out:
	return rc;
}