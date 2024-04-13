rio_interrupt (int irq, void *dev_instance)
{
	struct net_device *dev = dev_instance;
	struct netdev_private *np;
	unsigned int_status;
	long ioaddr;
	int cnt = max_intrloop;
	int handled = 0;

	ioaddr = dev->base_addr;
	np = netdev_priv(dev);
	while (1) {
		int_status = readw (ioaddr + IntStatus);
		writew (int_status, ioaddr + IntStatus);
		int_status &= DEFAULT_INTR;
		if (int_status == 0 || --cnt < 0)
			break;
		handled = 1;
		/* Processing received packets */
		if (int_status & RxDMAComplete)
			receive_packet (dev);
		/* TxDMAComplete interrupt */
		if ((int_status & (TxDMAComplete|IntRequested))) {
			int tx_status;
			tx_status = readl (ioaddr + TxStatus);
			if (tx_status & 0x01)
				tx_error (dev, tx_status);
			/* Free used tx skbuffs */
			rio_free_tx (dev, 1);
		}

		/* Handle uncommon events */
		if (int_status &
		    (HostError | LinkEvent | UpdateStats))
			rio_error (dev, int_status);
	}
	if (np->cur_tx != np->old_tx)
		writel (100, ioaddr + CountDown);
	return IRQ_RETVAL(handled);
}
