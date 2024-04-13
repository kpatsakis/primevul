static void iucv_process_message_q(struct sock *sk)
{
	struct iucv_sock *iucv = iucv_sk(sk);
	struct sk_buff *skb;
	struct sock_msg_q *p, *n;

	list_for_each_entry_safe(p, n, &iucv->message_q.list, list) {
		skb = alloc_skb(iucv_msg_length(&p->msg), GFP_ATOMIC | GFP_DMA);
		if (!skb)
			break;
		iucv_process_message(sk, skb, p->path, &p->msg);
		list_del(&p->list);
		kfree(p);
		if (!skb_queue_empty(&iucv->backlog_skb_q))
			break;
	}
}
