static int afiucv_hs_callback_win(struct sock *sk, struct sk_buff *skb)
{
	struct iucv_sock *iucv = iucv_sk(sk);
	struct af_iucv_trans_hdr *trans_hdr =
					(struct af_iucv_trans_hdr *)skb->data;

	if (!iucv)
		return NET_RX_SUCCESS;

	if (sk->sk_state != IUCV_CONNECTED)
		return NET_RX_SUCCESS;

	atomic_sub(trans_hdr->window, &iucv->msg_sent);
	iucv_sock_wake_msglim(sk);
	return NET_RX_SUCCESS;
}
