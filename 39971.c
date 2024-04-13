static int caif_queue_rcv_skb(struct sock *sk, struct sk_buff *skb)
{
	int err;
	int skb_len;
	unsigned long flags;
	struct sk_buff_head *list = &sk->sk_receive_queue;
	struct caifsock *cf_sk = container_of(sk, struct caifsock, sk);

	if (atomic_read(&sk->sk_rmem_alloc) + skb->truesize >=
		(unsigned int)sk->sk_rcvbuf && rx_flow_is_on(cf_sk)) {
		net_dbg_ratelimited("sending flow OFF (queue len = %d %d)\n",
				    atomic_read(&cf_sk->sk.sk_rmem_alloc),
				    sk_rcvbuf_lowwater(cf_sk));
		set_rx_flow_off(cf_sk);
		caif_flow_ctrl(sk, CAIF_MODEMCMD_FLOW_OFF_REQ);
	}

	err = sk_filter(sk, skb);
	if (err)
		return err;
	if (!sk_rmem_schedule(sk, skb, skb->truesize) && rx_flow_is_on(cf_sk)) {
		set_rx_flow_off(cf_sk);
		net_dbg_ratelimited("sending flow OFF due to rmem_schedule\n");
		caif_flow_ctrl(sk, CAIF_MODEMCMD_FLOW_OFF_REQ);
	}
	skb->dev = NULL;
	skb_set_owner_r(skb, sk);
	/* Cache the SKB length before we tack it onto the receive
	 * queue. Once it is added it no longer belongs to us and
	 * may be freed by other threads of control pulling packets
	 * from the queue.
	 */
	skb_len = skb->len;
	spin_lock_irqsave(&list->lock, flags);
	if (!sock_flag(sk, SOCK_DEAD))
		__skb_queue_tail(list, skb);
	spin_unlock_irqrestore(&list->lock, flags);

	if (!sock_flag(sk, SOCK_DEAD))
		sk->sk_data_ready(sk, skb_len);
	else
		kfree_skb(skb);
	return 0;
}
