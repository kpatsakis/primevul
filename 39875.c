static void l2tp_ip6_destroy_sock(struct sock *sk)
{
	struct l2tp_tunnel *tunnel = l2tp_sock_to_tunnel(sk);

	lock_sock(sk);
	ip6_flush_pending_frames(sk);
	release_sock(sk);

	if (tunnel) {
		l2tp_tunnel_closeall(tunnel);
		sock_put(sk);
	}

	inet6_destroy_sock(sk);
}
