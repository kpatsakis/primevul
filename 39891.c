static int afiucv_hs_callback_synfin(struct sock *sk, struct sk_buff *skb)
{
	struct iucv_sock *iucv = iucv_sk(sk);

	if (!iucv)
		goto out;
	if (sk->sk_state != IUCV_BOUND)
		goto out;
	bh_lock_sock(sk);
	sk->sk_state = IUCV_DISCONN;
	sk->sk_state_change(sk);
	bh_unlock_sock(sk);
out:
	kfree_skb(skb);
	return NET_RX_SUCCESS;
}
