static void qeth_notify_skbs(struct qeth_qdio_out_q *q,
		struct qeth_qdio_out_buffer *buf,
		enum iucv_tx_notify notification)
{
	struct sk_buff *skb;

	if (skb_queue_empty(&buf->skb_list))
		goto out;
	skb = skb_peek(&buf->skb_list);
	while (skb) {
		QETH_CARD_TEXT_(q->card, 5, "skbn%d", notification);
		QETH_CARD_TEXT_(q->card, 5, "%lx", (long) skb);
		if (skb->protocol == ETH_P_AF_IUCV) {
			if (skb->sk) {
				struct iucv_sock *iucv = iucv_sk(skb->sk);
				iucv->sk_txnotify(skb, notification);
			}
		}
		if (skb_queue_is_last(&buf->skb_list, skb))
			skb = NULL;
		else
			skb = skb_queue_next(&buf->skb_list, skb);
	}
out:
	return;
}
