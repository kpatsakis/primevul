void udpv6_destroy_sock(struct sock *sk)
{
	struct udp_sock *up = udp_sk(sk);
	lock_sock(sk);
	udp_v6_flush_pending_frames(sk);
	release_sock(sk);

	if (static_key_false(&udpv6_encap_needed) && up->encap_type) {
		void (*encap_destroy)(struct sock *sk);
		encap_destroy = ACCESS_ONCE(up->encap_destroy);
		if (encap_destroy)
			encap_destroy(sk);
	}

	inet6_destroy_sock(sk);
}
