static int afiucv_pm_restore_thaw(struct device *dev)
{
	struct sock *sk;

#ifdef CONFIG_PM_DEBUG
	printk(KERN_WARNING "afiucv_pm_restore_thaw\n");
#endif
	read_lock(&iucv_sk_list.lock);
	sk_for_each(sk, &iucv_sk_list.head) {
		switch (sk->sk_state) {
		case IUCV_CONNECTED:
			sk->sk_err = EPIPE;
			sk->sk_state = IUCV_DISCONN;
			sk->sk_state_change(sk);
			break;
		case IUCV_DISCONN:
		case IUCV_CLOSING:
		case IUCV_LISTEN:
		case IUCV_BOUND:
		case IUCV_OPEN:
		default:
			break;
		}
	}
	read_unlock(&iucv_sk_list.lock);
	return 0;
}
