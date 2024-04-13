alloc_list (struct net_device *dev)
{
	struct netdev_private *np = netdev_priv(dev);
	int i;

	np->cur_rx = np->cur_tx = 0;
	np->old_rx = np->old_tx = 0;
	np->rx_buf_sz = (dev->mtu <= 1500 ? PACKET_SIZE : dev->mtu + 32);

	/* Initialize Tx descriptors, TFDListPtr leaves in start_xmit(). */
	for (i = 0; i < TX_RING_SIZE; i++) {
		np->tx_skbuff[i] = NULL;
		np->tx_ring[i].status = cpu_to_le64 (TFDDone);
		np->tx_ring[i].next_desc = cpu_to_le64 (np->tx_ring_dma +
					      ((i+1)%TX_RING_SIZE) *
					      sizeof (struct netdev_desc));
	}

	/* Initialize Rx descriptors */
	for (i = 0; i < RX_RING_SIZE; i++) {
		np->rx_ring[i].next_desc = cpu_to_le64 (np->rx_ring_dma +
						((i + 1) % RX_RING_SIZE) *
						sizeof (struct netdev_desc));
		np->rx_ring[i].status = 0;
		np->rx_ring[i].fraginfo = 0;
		np->rx_skbuff[i] = NULL;
	}

	/* Allocate the rx buffers */
	for (i = 0; i < RX_RING_SIZE; i++) {
		/* Allocated fixed size of skbuff */
		struct sk_buff *skb;

		skb = netdev_alloc_skb_ip_align(dev, np->rx_buf_sz);
		np->rx_skbuff[i] = skb;
		if (skb == NULL) {
			printk (KERN_ERR
				"%s: alloc_list: allocate Rx buffer error! ",
				dev->name);
			break;
		}
		/* Rubicon now supports 40 bits of addressing space. */
		np->rx_ring[i].fraginfo =
		    cpu_to_le64 ( pci_map_single (
			 	  np->pdev, skb->data, np->rx_buf_sz,
				  PCI_DMA_FROMDEVICE));
		np->rx_ring[i].fraginfo |= cpu_to_le64((u64)np->rx_buf_sz << 48);
	}

	/* Set RFDListPtr */
	writel (np->rx_ring_dma, dev->base_addr + RFDListPtr0);
	writel (0, dev->base_addr + RFDListPtr1);
}
