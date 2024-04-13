static u16 tun_select_queue(struct net_device *dev, struct sk_buff *skb,
			    void *accel_priv, select_queue_fallback_t fallback)
{
	struct tun_struct *tun = netdev_priv(dev);
	struct tun_flow_entry *e;
	u32 txq = 0;
	u32 numqueues = 0;

	rcu_read_lock();
	numqueues = ACCESS_ONCE(tun->numqueues);

	txq = __skb_get_hash_symmetric(skb);
	if (txq) {
		e = tun_flow_find(&tun->flows[tun_hashfn(txq)], txq);
		if (e) {
			tun_flow_save_rps_rxhash(e, txq);
			txq = e->queue_index;
		} else
			/* use multiply and shift instead of expensive divide */
			txq = ((u64)txq * numqueues) >> 32;
	} else if (likely(skb_rx_queue_recorded(skb))) {
		txq = skb_get_rx_queue(skb);
		while (unlikely(txq >= numqueues))
			txq -= numqueues;
	}

	rcu_read_unlock();
	return txq;
}
