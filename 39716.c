static int vsock_queue_rcv_skb(struct sock *sk, struct sk_buff *skb)
{
	int err;

	err = sock_queue_rcv_skb(sk, skb);
	if (err)
		kfree_skb(skb);

	return err;
}
