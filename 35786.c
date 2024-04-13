rio_error (struct net_device *dev, int int_status)
{
	long ioaddr = dev->base_addr;
	struct netdev_private *np = netdev_priv(dev);
	u16 macctrl;

	/* Link change event */
	if (int_status & LinkEvent) {
		if (mii_wait_link (dev, 10) == 0) {
			printk (KERN_INFO "%s: Link up\n", dev->name);
			if (np->phy_media)
				mii_get_media_pcs (dev);
			else
				mii_get_media (dev);
			if (np->speed == 1000)
				np->tx_coalesce = tx_coalesce;
			else
				np->tx_coalesce = 1;
			macctrl = 0;
			macctrl |= (np->vlan) ? AutoVLANuntagging : 0;
			macctrl |= (np->full_duplex) ? DuplexSelect : 0;
			macctrl |= (np->tx_flow) ?
				TxFlowControlEnable : 0;
			macctrl |= (np->rx_flow) ?
				RxFlowControlEnable : 0;
			writew(macctrl,	ioaddr + MACCtrl);
			np->link_status = 1;
			netif_carrier_on(dev);
		} else {
			printk (KERN_INFO "%s: Link off\n", dev->name);
			np->link_status = 0;
			netif_carrier_off(dev);
		}
	}

	/* UpdateStats statistics registers */
	if (int_status & UpdateStats) {
		get_stats (dev);
	}

	/* PCI Error, a catastronphic error related to the bus interface
	   occurs, set GlobalReset and HostReset to reset. */
	if (int_status & HostError) {
		printk (KERN_ERR "%s: HostError! IntStatus %4.4x.\n",
			dev->name, int_status);
		writew (GlobalReset | HostReset, ioaddr + ASICCtrl + 2);
		mdelay (500);
	}
}
