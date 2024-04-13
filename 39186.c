void ip_send_unicast_reply(struct net *net, struct sk_buff *skb, __be32 daddr,
			   __be32 saddr, const struct ip_reply_arg *arg,
			   unsigned int len)
{
	struct ip_options_data replyopts;
	struct ipcm_cookie ipc;
	struct flowi4 fl4;
	struct rtable *rt = skb_rtable(skb);
	struct sk_buff *nskb;
	struct sock *sk;
	struct inet_sock *inet;

	if (ip_options_echo(&replyopts.opt.opt, skb))
		return;

	ipc.addr = daddr;
	ipc.opt = NULL;
	ipc.tx_flags = 0;

	if (replyopts.opt.opt.optlen) {
		ipc.opt = &replyopts.opt;

		if (replyopts.opt.opt.srr)
			daddr = replyopts.opt.opt.faddr;
	}

	flowi4_init_output(&fl4, arg->bound_dev_if, 0,
			   RT_TOS(arg->tos),
			   RT_SCOPE_UNIVERSE, ip_hdr(skb)->protocol,
			   ip_reply_arg_flowi_flags(arg),
			   daddr, saddr,
			   tcp_hdr(skb)->source, tcp_hdr(skb)->dest);
	security_skb_classify_flow(skb, flowi4_to_flowi(&fl4));
	rt = ip_route_output_key(net, &fl4);
	if (IS_ERR(rt))
		return;

	inet = &get_cpu_var(unicast_sock);

	inet->tos = arg->tos;
	sk = &inet->sk;
	sk->sk_priority = skb->priority;
	sk->sk_protocol = ip_hdr(skb)->protocol;
	sk->sk_bound_dev_if = arg->bound_dev_if;
	sock_net_set(sk, net);
	__skb_queue_head_init(&sk->sk_write_queue);
	sk->sk_sndbuf = sysctl_wmem_default;
	ip_append_data(sk, &fl4, ip_reply_glue_bits, arg->iov->iov_base, len, 0,
		       &ipc, &rt, MSG_DONTWAIT);
	nskb = skb_peek(&sk->sk_write_queue);
	if (nskb) {
		if (arg->csumoffset >= 0)
			*((__sum16 *)skb_transport_header(nskb) +
			  arg->csumoffset) = csum_fold(csum_add(nskb->csum,
								arg->csum));
		nskb->ip_summed = CHECKSUM_NONE;
		skb_orphan(nskb);
		skb_set_queue_mapping(nskb, skb_get_queue_mapping(skb));
		ip_push_pending_frames(sk, &fl4);
	}

	put_cpu_var(unicast_sock);

	ip_rt_put(rt);
}
