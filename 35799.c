start_xmit (struct sk_buff *skb, struct net_device *dev)
{
	struct netdev_private *np = netdev_priv(dev);
	struct netdev_desc *txdesc;
	unsigned entry;
	u32 ioaddr;
	u64 tfc_vlan_tag = 0;

	if (np->link_status == 0) {	/* Link Down */
		dev_kfree_skb(skb);
		return NETDEV_TX_OK;
	}
	ioaddr = dev->base_addr;
	entry = np->cur_tx % TX_RING_SIZE;
	np->tx_skbuff[entry] = skb;
	txdesc = &np->tx_ring[entry];

#if 0
	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		txdesc->status |=
		    cpu_to_le64 (TCPChecksumEnable | UDPChecksumEnable |
				 IPChecksumEnable);
	}
#endif
	if (np->vlan) {
		tfc_vlan_tag = VLANTagInsert |
		    ((u64)np->vlan << 32) |
		    ((u64)skb->priority << 45);
	}
	txdesc->fraginfo = cpu_to_le64 (pci_map_single (np->pdev, skb->data,
							skb->len,
							PCI_DMA_TODEVICE));
	txdesc->fraginfo |= cpu_to_le64((u64)skb->len << 48);

	/* DL2K bug: DMA fails to get next descriptor ptr in 10Mbps mode
	 * Work around: Always use 1 descriptor in 10Mbps mode */
	if (entry % np->tx_coalesce == 0 || np->speed == 10)
		txdesc->status = cpu_to_le64 (entry | tfc_vlan_tag |
					      WordAlignDisable |
					      TxDMAIndicate |
					      (1 << FragCountShift));
	else
		txdesc->status = cpu_to_le64 (entry | tfc_vlan_tag |
					      WordAlignDisable |
					      (1 << FragCountShift));

	/* TxDMAPollNow */
	writel (readl (ioaddr + DMACtrl) | 0x00001000, ioaddr + DMACtrl);
	/* Schedule ISR */
	writel(10000, ioaddr + CountDown);
	np->cur_tx = (np->cur_tx + 1) % TX_RING_SIZE;
	if ((np->cur_tx - np->old_tx + TX_RING_SIZE) % TX_RING_SIZE
			< TX_QUEUE_LEN - 1 && np->speed != 10) {
		/* do nothing */
	} else if (!netif_queue_stopped(dev)) {
		netif_stop_queue (dev);
	}

	/* The first TFDListPtr */
	if (readl (dev->base_addr + TFDListPtr0) == 0) {
		writel (np->tx_ring_dma + entry * sizeof (struct netdev_desc),
			dev->base_addr + TFDListPtr0);
		writel (0, dev->base_addr + TFDListPtr1);
	}

	return NETDEV_TX_OK;
}
