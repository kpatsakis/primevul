static int l2tp_ip6_push_pending_frames(struct sock *sk)
{
	struct sk_buff *skb;
	__be32 *transhdr = NULL;
	int err = 0;

	skb = skb_peek(&sk->sk_write_queue);
	if (skb == NULL)
		goto out;

	transhdr = (__be32 *)skb_transport_header(skb);
	*transhdr = 0;

	err = ip6_push_pending_frames(sk);

out:
	return err;
}
