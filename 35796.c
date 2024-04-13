rio_timer (unsigned long data)
{
	struct net_device *dev = (struct net_device *)data;
	struct netdev_private *np = netdev_priv(dev);
	unsigned int entry;
	int next_tick = 1*HZ;
	unsigned long flags;

	spin_lock_irqsave(&np->rx_lock, flags);
	/* Recover rx ring exhausted error */
	if (np->cur_rx - np->old_rx >= RX_RING_SIZE) {
		printk(KERN_INFO "Try to recover rx ring exhausted...\n");
		/* Re-allocate skbuffs to fill the descriptor ring */
		for (; np->cur_rx - np->old_rx > 0; np->old_rx++) {
			struct sk_buff *skb;
			entry = np->old_rx % RX_RING_SIZE;
			/* Dropped packets don't need to re-allocate */
			if (np->rx_skbuff[entry] == NULL) {
				skb = netdev_alloc_skb_ip_align(dev,
								np->rx_buf_sz);
				if (skb == NULL) {
					np->rx_ring[entry].fraginfo = 0;
					printk (KERN_INFO
						"%s: Still unable to re-allocate Rx skbuff.#%d\n",
						dev->name, entry);
					break;
				}
				np->rx_skbuff[entry] = skb;
				np->rx_ring[entry].fraginfo =
				    cpu_to_le64 (pci_map_single
					 (np->pdev, skb->data, np->rx_buf_sz,
					  PCI_DMA_FROMDEVICE));
			}
			np->rx_ring[entry].fraginfo |=
			    cpu_to_le64((u64)np->rx_buf_sz << 48);
			np->rx_ring[entry].status = 0;
		} /* end for */
	} /* end if */
	spin_unlock_irqrestore (&np->rx_lock, flags);
	np->timer.expires = jiffies + next_tick;
	add_timer(&np->timer);
}
