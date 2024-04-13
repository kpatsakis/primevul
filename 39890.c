static int afiucv_hs_callback_synack(struct sock *sk, struct sk_buff *skb)
{
	struct iucv_sock *iucv = iucv_sk(sk);
	struct af_iucv_trans_hdr *trans_hdr =
					(struct af_iucv_trans_hdr *)skb->data;

	if (!iucv)
		goto out;
	if (sk->sk_state != IUCV_BOUND)
		goto out;
	bh_lock_sock(sk);
	iucv->msglimit_peer = trans_hdr->window;
	sk->sk_state = IUCV_CONNECTED;
	sk->sk_state_change(sk);
	bh_unlock_sock(sk);
out:
	kfree_skb(skb);
	return NET_RX_SUCCESS;
}
