static int afiucv_hs_callback_syn(struct sock *sk, struct sk_buff *skb)
{
	struct sock *nsk;
	struct iucv_sock *iucv, *niucv;
	struct af_iucv_trans_hdr *trans_hdr;
	int err;

	iucv = iucv_sk(sk);
	trans_hdr = (struct af_iucv_trans_hdr *)skb->data;
	if (!iucv) {
		/* no sock - connection refused */
		afiucv_swap_src_dest(skb);
		trans_hdr->flags = AF_IUCV_FLAG_SYN | AF_IUCV_FLAG_FIN;
		err = dev_queue_xmit(skb);
		goto out;
	}

	nsk = iucv_sock_alloc(NULL, sk->sk_type, GFP_ATOMIC);
	bh_lock_sock(sk);
	if ((sk->sk_state != IUCV_LISTEN) ||
	    sk_acceptq_is_full(sk) ||
	    !nsk) {
		/* error on server socket - connection refused */
		if (nsk)
			sk_free(nsk);
		afiucv_swap_src_dest(skb);
		trans_hdr->flags = AF_IUCV_FLAG_SYN | AF_IUCV_FLAG_FIN;
		err = dev_queue_xmit(skb);
		bh_unlock_sock(sk);
		goto out;
	}

	niucv = iucv_sk(nsk);
	iucv_sock_init(nsk, sk);
	niucv->transport = AF_IUCV_TRANS_HIPER;
	niucv->msglimit = iucv->msglimit;
	if (!trans_hdr->window)
		niucv->msglimit_peer = IUCV_HIPER_MSGLIM_DEFAULT;
	else
		niucv->msglimit_peer = trans_hdr->window;
	memcpy(niucv->dst_name, trans_hdr->srcAppName, 8);
	memcpy(niucv->dst_user_id, trans_hdr->srcUserID, 8);
	memcpy(niucv->src_name, iucv->src_name, 8);
	memcpy(niucv->src_user_id, iucv->src_user_id, 8);
	nsk->sk_bound_dev_if = sk->sk_bound_dev_if;
	niucv->hs_dev = iucv->hs_dev;
	dev_hold(niucv->hs_dev);
	afiucv_swap_src_dest(skb);
	trans_hdr->flags = AF_IUCV_FLAG_SYN | AF_IUCV_FLAG_ACK;
	trans_hdr->window = niucv->msglimit;
	/* if receiver acks the xmit connection is established */
	err = dev_queue_xmit(skb);
	if (!err) {
		iucv_accept_enqueue(sk, nsk);
		nsk->sk_state = IUCV_CONNECTED;
		sk->sk_data_ready(sk, 1);
	} else
		iucv_sock_kill(nsk);
	bh_unlock_sock(sk);

out:
	return NET_RX_SUCCESS;
}
