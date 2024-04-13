mii_get_media_pcs (struct net_device *dev)
{
	__u16 negotiate;
	__u16 bmsr;
	int phy_addr;
	struct netdev_private *np;

	np = netdev_priv(dev);
	phy_addr = np->phy_addr;

	bmsr = mii_read (dev, phy_addr, PCS_BMSR);
	if (np->an_enable) {
		if (!(bmsr & BMSR_ANEGCOMPLETE)) {
			/* Auto-Negotiation not completed */
			return -1;
		}
		negotiate = mii_read (dev, phy_addr, PCS_ANAR) &
			mii_read (dev, phy_addr, PCS_ANLPAR);
		np->speed = 1000;
		if (negotiate & PCS_ANAR_FULL_DUPLEX) {
			printk (KERN_INFO "Auto 1000 Mbps, Full duplex\n");
			np->full_duplex = 1;
		} else {
			printk (KERN_INFO "Auto 1000 Mbps, half duplex\n");
			np->full_duplex = 0;
		}
		if (negotiate & PCS_ANAR_PAUSE) {
			np->tx_flow &= 1;
			np->rx_flow &= 1;
		} else if (negotiate & PCS_ANAR_ASYMMETRIC) {
			np->tx_flow = 0;
			np->rx_flow &= 1;
		}
		/* else tx_flow, rx_flow = user select  */
	} else {
		__u16 bmcr = mii_read (dev, phy_addr, PCS_BMCR);
		printk (KERN_INFO "Operating at 1000 Mbps, ");
		if (bmcr & BMCR_FULLDPLX) {
			printk (KERN_CONT "Full duplex\n");
		} else {
			printk (KERN_CONT "Half duplex\n");
		}
	}
	if (np->tx_flow)
		printk(KERN_INFO "Enable Tx Flow Control\n");
	else
		printk(KERN_INFO "Disable Tx Flow Control\n");
	if (np->rx_flow)
		printk(KERN_INFO "Enable Rx Flow Control\n");
	else
		printk(KERN_INFO "Disable Rx Flow Control\n");

	return 0;
}
