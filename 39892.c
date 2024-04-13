static void afiucv_hs_callback_txnotify(struct sk_buff *skb,
					enum iucv_tx_notify n)
{
	struct sock *isk = skb->sk;
	struct sock *sk = NULL;
	struct iucv_sock *iucv = NULL;
	struct sk_buff_head *list;
	struct sk_buff *list_skb;
	struct sk_buff *nskb;
	unsigned long flags;

	read_lock_irqsave(&iucv_sk_list.lock, flags);
	sk_for_each(sk, &iucv_sk_list.head)
		if (sk == isk) {
			iucv = iucv_sk(sk);
			break;
		}
	read_unlock_irqrestore(&iucv_sk_list.lock, flags);

	if (!iucv || sock_flag(sk, SOCK_ZAPPED))
		return;

	list = &iucv->send_skb_q;
	spin_lock_irqsave(&list->lock, flags);
	if (skb_queue_empty(list))
		goto out_unlock;
	list_skb = list->next;
	nskb = list_skb->next;
	while (list_skb != (struct sk_buff *)list) {
		if (skb_shinfo(list_skb) == skb_shinfo(skb)) {
			switch (n) {
			case TX_NOTIFY_OK:
				__skb_unlink(list_skb, list);
				kfree_skb(list_skb);
				iucv_sock_wake_msglim(sk);
				break;
			case TX_NOTIFY_PENDING:
				atomic_inc(&iucv->pendings);
				break;
			case TX_NOTIFY_DELAYED_OK:
				__skb_unlink(list_skb, list);
				atomic_dec(&iucv->pendings);
				if (atomic_read(&iucv->pendings) <= 0)
					iucv_sock_wake_msglim(sk);
				kfree_skb(list_skb);
				break;
			case TX_NOTIFY_UNREACHABLE:
			case TX_NOTIFY_DELAYED_UNREACHABLE:
			case TX_NOTIFY_TPQFULL: /* not yet used */
			case TX_NOTIFY_GENERALERROR:
			case TX_NOTIFY_DELAYED_GENERALERROR:
				__skb_unlink(list_skb, list);
				kfree_skb(list_skb);
				if (sk->sk_state == IUCV_CONNECTED) {
					sk->sk_state = IUCV_DISCONN;
					sk->sk_state_change(sk);
				}
				break;
			}
			break;
		}
		list_skb = nskb;
		nskb = nskb->next;
	}
out_unlock:
	spin_unlock_irqrestore(&list->lock, flags);

	if (sk->sk_state == IUCV_CLOSING) {
		if (skb_queue_empty(&iucv_sk(sk)->send_skb_q)) {
			sk->sk_state = IUCV_CLOSED;
			sk->sk_state_change(sk);
		}
	}

}
