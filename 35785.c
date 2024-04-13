rio_close (struct net_device *dev)
{
	long ioaddr = dev->base_addr;
	struct netdev_private *np = netdev_priv(dev);
	struct sk_buff *skb;
	int i;

	netif_stop_queue (dev);

	/* Disable interrupts */
	writew (0, ioaddr + IntEnable);

	/* Stop Tx and Rx logics */
	writel (TxDisable | RxDisable | StatsDisable, ioaddr + MACCtrl);

	free_irq (dev->irq, dev);
	del_timer_sync (&np->timer);

	/* Free all the skbuffs in the queue. */
	for (i = 0; i < RX_RING_SIZE; i++) {
		skb = np->rx_skbuff[i];
		if (skb) {
			pci_unmap_single(np->pdev,
					 desc_to_dma(&np->rx_ring[i]),
					 skb->len, PCI_DMA_FROMDEVICE);
			dev_kfree_skb (skb);
			np->rx_skbuff[i] = NULL;
		}
		np->rx_ring[i].status = 0;
		np->rx_ring[i].fraginfo = 0;
	}
	for (i = 0; i < TX_RING_SIZE; i++) {
		skb = np->tx_skbuff[i];
		if (skb) {
			pci_unmap_single(np->pdev,
					 desc_to_dma(&np->tx_ring[i]),
					 skb->len, PCI_DMA_TODEVICE);
			dev_kfree_skb (skb);
			np->tx_skbuff[i] = NULL;
		}
	}

	return 0;
}
