void ping_rcv(struct sk_buff *skb)
{
	struct sock *sk;
	struct net *net = dev_net(skb->dev);
	struct icmphdr *icmph = icmp_hdr(skb);

	/* We assume the packet has already been checked by icmp_rcv */

	pr_debug("ping_rcv(skb=%p,id=%04x,seq=%04x)\n",
		 skb, ntohs(icmph->un.echo.id), ntohs(icmph->un.echo.sequence));

	/* Push ICMP header back */
	skb_push(skb, skb->data - (u8 *)icmph);

	sk = ping_lookup(net, skb, ntohs(icmph->un.echo.id));
	if (sk != NULL) {
		pr_debug("rcv on socket %p\n", sk);
		ping_queue_rcv_skb(sk, skb_get(skb));
		sock_put(sk);
		return;
	}
	pr_debug("no socket, dropping\n");

	/* We're called from icmp_rcv(). kfree_skb() is done there. */
}
