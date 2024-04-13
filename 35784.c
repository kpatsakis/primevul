receive_packet (struct net_device *dev)
{
	struct netdev_private *np = netdev_priv(dev);
	int entry = np->cur_rx % RX_RING_SIZE;
	int cnt = 30;

	/* If RFDDone, FrameStart and FrameEnd set, there is a new packet in. */
	while (1) {
		struct netdev_desc *desc = &np->rx_ring[entry];
		int pkt_len;
		u64 frame_status;

		if (!(desc->status & cpu_to_le64(RFDDone)) ||
		    !(desc->status & cpu_to_le64(FrameStart)) ||
		    !(desc->status & cpu_to_le64(FrameEnd)))
			break;

		/* Chip omits the CRC. */
		frame_status = le64_to_cpu(desc->status);
		pkt_len = frame_status & 0xffff;
		if (--cnt < 0)
			break;
		/* Update rx error statistics, drop packet. */
		if (frame_status & RFS_Errors) {
			np->stats.rx_errors++;
			if (frame_status & (RxRuntFrame | RxLengthError))
				np->stats.rx_length_errors++;
			if (frame_status & RxFCSError)
				np->stats.rx_crc_errors++;
			if (frame_status & RxAlignmentError && np->speed != 1000)
				np->stats.rx_frame_errors++;
			if (frame_status & RxFIFOOverrun)
	 			np->stats.rx_fifo_errors++;
		} else {
			struct sk_buff *skb;

			/* Small skbuffs for short packets */
			if (pkt_len > copy_thresh) {
				pci_unmap_single (np->pdev,
						  desc_to_dma(desc),
						  np->rx_buf_sz,
						  PCI_DMA_FROMDEVICE);
				skb_put (skb = np->rx_skbuff[entry], pkt_len);
				np->rx_skbuff[entry] = NULL;
			} else if ((skb = netdev_alloc_skb_ip_align(dev, pkt_len))) {
				pci_dma_sync_single_for_cpu(np->pdev,
							    desc_to_dma(desc),
							    np->rx_buf_sz,
							    PCI_DMA_FROMDEVICE);
				skb_copy_to_linear_data (skb,
						  np->rx_skbuff[entry]->data,
						  pkt_len);
				skb_put (skb, pkt_len);
				pci_dma_sync_single_for_device(np->pdev,
							       desc_to_dma(desc),
							       np->rx_buf_sz,
							       PCI_DMA_FROMDEVICE);
			}
			skb->protocol = eth_type_trans (skb, dev);
#if 0
			/* Checksum done by hw, but csum value unavailable. */
			if (np->pdev->pci_rev_id >= 0x0c &&
				!(frame_status & (TCPError | UDPError | IPError))) {
				skb->ip_summed = CHECKSUM_UNNECESSARY;
			}
#endif
			netif_rx (skb);
		}
		entry = (entry + 1) % RX_RING_SIZE;
	}
	spin_lock(&np->rx_lock);
	np->cur_rx = entry;
	/* Re-allocate skbuffs to fill the descriptor ring */
	entry = np->old_rx;
	while (entry != np->cur_rx) {
		struct sk_buff *skb;
		/* Dropped packets don't need to re-allocate */
		if (np->rx_skbuff[entry] == NULL) {
			skb = netdev_alloc_skb_ip_align(dev, np->rx_buf_sz);
			if (skb == NULL) {
				np->rx_ring[entry].fraginfo = 0;
				printk (KERN_INFO
					"%s: receive_packet: "
					"Unable to re-allocate Rx skbuff.#%d\n",
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
		entry = (entry + 1) % RX_RING_SIZE;
	}
	np->old_rx = entry;
	spin_unlock(&np->rx_lock);
	return 0;
}
