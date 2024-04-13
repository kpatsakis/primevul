static int afiucv_pm_freeze(struct device *dev)
{
	struct iucv_sock *iucv;
	struct sock *sk;
	int err = 0;

#ifdef CONFIG_PM_DEBUG
	printk(KERN_WARNING "afiucv_pm_freeze\n");
#endif
	read_lock(&iucv_sk_list.lock);
	sk_for_each(sk, &iucv_sk_list.head) {
		iucv = iucv_sk(sk);
		switch (sk->sk_state) {
		case IUCV_DISCONN:
		case IUCV_CLOSING:
		case IUCV_CONNECTED:
			iucv_sever_path(sk, 0);
			break;
		case IUCV_OPEN:
		case IUCV_BOUND:
		case IUCV_LISTEN:
		case IUCV_CLOSED:
		default:
			break;
		}
		skb_queue_purge(&iucv->send_skb_q);
		skb_queue_purge(&iucv->backlog_skb_q);
	}
	read_unlock(&iucv_sk_list.lock);
	return err;
}
