rio_probe1 (struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct net_device *dev;
	struct netdev_private *np;
	static int card_idx;
	int chip_idx = ent->driver_data;
	int err, irq;
	long ioaddr;
	static int version_printed;
	void *ring_space;
	dma_addr_t ring_dma;

	if (!version_printed++)
		printk ("%s", version);

	err = pci_enable_device (pdev);
	if (err)
		return err;

	irq = pdev->irq;
	err = pci_request_regions (pdev, "dl2k");
	if (err)
		goto err_out_disable;

	pci_set_master (pdev);
	dev = alloc_etherdev (sizeof (*np));
	if (!dev) {
		err = -ENOMEM;
		goto err_out_res;
	}
	SET_NETDEV_DEV(dev, &pdev->dev);

#ifdef MEM_MAPPING
	ioaddr = pci_resource_start (pdev, 1);
	ioaddr = (long) ioremap (ioaddr, RIO_IO_SIZE);
	if (!ioaddr) {
		err = -ENOMEM;
		goto err_out_dev;
	}
#else
	ioaddr = pci_resource_start (pdev, 0);
#endif
	dev->base_addr = ioaddr;
	dev->irq = irq;
	np = netdev_priv(dev);
	np->chip_id = chip_idx;
	np->pdev = pdev;
	spin_lock_init (&np->tx_lock);
	spin_lock_init (&np->rx_lock);

	/* Parse manual configuration */
	np->an_enable = 1;
	np->tx_coalesce = 1;
	if (card_idx < MAX_UNITS) {
		if (media[card_idx] != NULL) {
			np->an_enable = 0;
			if (strcmp (media[card_idx], "auto") == 0 ||
			    strcmp (media[card_idx], "autosense") == 0 ||
			    strcmp (media[card_idx], "0") == 0 ) {
				np->an_enable = 2;
			} else if (strcmp (media[card_idx], "100mbps_fd") == 0 ||
			    strcmp (media[card_idx], "4") == 0) {
				np->speed = 100;
				np->full_duplex = 1;
			} else if (strcmp (media[card_idx], "100mbps_hd") == 0 ||
				   strcmp (media[card_idx], "3") == 0) {
				np->speed = 100;
				np->full_duplex = 0;
			} else if (strcmp (media[card_idx], "10mbps_fd") == 0 ||
				   strcmp (media[card_idx], "2") == 0) {
				np->speed = 10;
				np->full_duplex = 1;
			} else if (strcmp (media[card_idx], "10mbps_hd") == 0 ||
				   strcmp (media[card_idx], "1") == 0) {
				np->speed = 10;
				np->full_duplex = 0;
			} else if (strcmp (media[card_idx], "1000mbps_fd") == 0 ||
				 strcmp (media[card_idx], "6") == 0) {
				np->speed=1000;
				np->full_duplex=1;
			} else if (strcmp (media[card_idx], "1000mbps_hd") == 0 ||
				 strcmp (media[card_idx], "5") == 0) {
				np->speed = 1000;
				np->full_duplex = 0;
			} else {
				np->an_enable = 1;
			}
		}
		if (jumbo[card_idx] != 0) {
			np->jumbo = 1;
			dev->mtu = MAX_JUMBO;
		} else {
			np->jumbo = 0;
			if (mtu[card_idx] > 0 && mtu[card_idx] < PACKET_SIZE)
				dev->mtu = mtu[card_idx];
		}
		np->vlan = (vlan[card_idx] > 0 && vlan[card_idx] < 4096) ?
		    vlan[card_idx] : 0;
		if (rx_coalesce > 0 && rx_timeout > 0) {
			np->rx_coalesce = rx_coalesce;
			np->rx_timeout = rx_timeout;
			np->coalesce = 1;
		}
		np->tx_flow = (tx_flow == 0) ? 0 : 1;
		np->rx_flow = (rx_flow == 0) ? 0 : 1;

		if (tx_coalesce < 1)
			tx_coalesce = 1;
		else if (tx_coalesce > TX_RING_SIZE-1)
			tx_coalesce = TX_RING_SIZE - 1;
	}
	dev->netdev_ops = &netdev_ops;
	dev->watchdog_timeo = TX_TIMEOUT;
	SET_ETHTOOL_OPS(dev, &ethtool_ops);
#if 0
	dev->features = NETIF_F_IP_CSUM;
#endif
	pci_set_drvdata (pdev, dev);

	ring_space = pci_alloc_consistent (pdev, TX_TOTAL_SIZE, &ring_dma);
	if (!ring_space)
		goto err_out_iounmap;
	np->tx_ring = ring_space;
	np->tx_ring_dma = ring_dma;

	ring_space = pci_alloc_consistent (pdev, RX_TOTAL_SIZE, &ring_dma);
	if (!ring_space)
		goto err_out_unmap_tx;
	np->rx_ring = ring_space;
	np->rx_ring_dma = ring_dma;

	/* Parse eeprom data */
	parse_eeprom (dev);

	/* Find PHY address */
	err = find_miiphy (dev);
	if (err)
		goto err_out_unmap_rx;

	/* Fiber device? */
	np->phy_media = (readw(ioaddr + ASICCtrl) & PhyMedia) ? 1 : 0;
	np->link_status = 0;
	/* Set media and reset PHY */
	if (np->phy_media) {
		/* default Auto-Negotiation for fiber deivices */
	 	if (np->an_enable == 2) {
			np->an_enable = 1;
		}
		mii_set_media_pcs (dev);
	} else {
		/* Auto-Negotiation is mandatory for 1000BASE-T,
		   IEEE 802.3ab Annex 28D page 14 */
		if (np->speed == 1000)
			np->an_enable = 1;
		mii_set_media (dev);
	}

	err = register_netdev (dev);
	if (err)
		goto err_out_unmap_rx;

	card_idx++;

	printk (KERN_INFO "%s: %s, %pM, IRQ %d\n",
		dev->name, np->name, dev->dev_addr, irq);
	if (tx_coalesce > 1)
		printk(KERN_INFO "tx_coalesce:\t%d packets\n",
				tx_coalesce);
	if (np->coalesce)
		printk(KERN_INFO
		       "rx_coalesce:\t%d packets\n"
		       "rx_timeout: \t%d ns\n",
				np->rx_coalesce, np->rx_timeout*640);
	if (np->vlan)
		printk(KERN_INFO "vlan(id):\t%d\n", np->vlan);
	return 0;

      err_out_unmap_rx:
	pci_free_consistent (pdev, RX_TOTAL_SIZE, np->rx_ring, np->rx_ring_dma);
      err_out_unmap_tx:
	pci_free_consistent (pdev, TX_TOTAL_SIZE, np->tx_ring, np->tx_ring_dma);
      err_out_iounmap:
#ifdef MEM_MAPPING
	iounmap ((void *) ioaddr);

      err_out_dev:
#endif
	free_netdev (dev);

      err_out_res:
	pci_release_regions (pdev);

      err_out_disable:
	pci_disable_device (pdev);
	return err;
}
