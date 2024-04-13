static int iucv_send_ctrl(struct sock *sk, u8 flags)
{
	int err = 0;
	int blen;
	struct sk_buff *skb;

	blen = sizeof(struct af_iucv_trans_hdr) + ETH_HLEN;
	skb = sock_alloc_send_skb(sk, blen, 1, &err);
	if (skb) {
		skb_reserve(skb, blen);
		err = afiucv_hs_send(NULL, sk, skb, flags);
	}
	return err;
}
