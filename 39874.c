static int l2tp_ip6_connect(struct sock *sk, struct sockaddr *uaddr,
			    int addr_len)
{
	struct sockaddr_l2tpip6 *lsa = (struct sockaddr_l2tpip6 *) uaddr;
	struct sockaddr_in6	*usin = (struct sockaddr_in6 *) uaddr;
	struct in6_addr	*daddr;
	int	addr_type;
	int rc;

	if (sock_flag(sk, SOCK_ZAPPED)) /* Must bind first - autobinding does not work */
		return -EINVAL;

	if (addr_len < sizeof(*lsa))
		return -EINVAL;

	addr_type = ipv6_addr_type(&usin->sin6_addr);
	if (addr_type & IPV6_ADDR_MULTICAST)
		return -EINVAL;

	if (addr_type & IPV6_ADDR_MAPPED) {
		daddr = &usin->sin6_addr;
		if (ipv4_is_multicast(daddr->s6_addr32[3]))
			return -EINVAL;
	}

	rc = ip6_datagram_connect(sk, uaddr, addr_len);

	lock_sock(sk);

	l2tp_ip6_sk(sk)->peer_conn_id = lsa->l2tp_conn_id;

	write_lock_bh(&l2tp_ip6_lock);
	hlist_del_init(&sk->sk_bind_node);
	sk_add_bind_node(sk, &l2tp_ip6_bind_table);
	write_unlock_bh(&l2tp_ip6_lock);

	release_sock(sk);

	return rc;
}
