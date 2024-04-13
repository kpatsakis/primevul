mii_get_media (struct net_device *dev)
{
	__u16 negotiate;
	__u16 bmsr;
	__u16 mscr;
	__u16 mssr;
	int phy_addr;
	struct netdev_private *np;

	np = netdev_priv(dev);
	phy_addr = np->phy_addr;

	bmsr = mii_read (dev, phy_addr, MII_BMSR);
	if (np->an_enable) {
		if (!(bmsr & BMSR_ANEGCOMPLETE)) {
			/* Auto-Negotiation not completed */
			return -1;
		}
		negotiate = mii_read (dev, phy_addr, MII_ADVERTISE) &
			mii_read (dev, phy_addr, MII_LPA);
		mscr = mii_read (dev, phy_addr, MII_CTRL1000);
		mssr = mii_read (dev, phy_addr, MII_STAT1000);
		if (mscr & ADVERTISE_1000FULL && mssr & LPA_1000FULL) {
			np->speed = 1000;
			np->full_duplex = 1;
			printk (KERN_INFO "Auto 1000 Mbps, Full duplex\n");
		} else if (mscr & ADVERTISE_1000HALF && mssr & LPA_1000HALF) {
			np->speed = 1000;
			np->full_duplex = 0;
			printk (KERN_INFO "Auto 1000 Mbps, Half duplex\n");
		} else if (negotiate & ADVERTISE_100FULL) {
			np->speed = 100;
			np->full_duplex = 1;
			printk (KERN_INFO "Auto 100 Mbps, Full duplex\n");
		} else if (negotiate & ADVERTISE_100HALF) {
			np->speed = 100;
			np->full_duplex = 0;
			printk (KERN_INFO "Auto 100 Mbps, Half duplex\n");
		} else if (negotiate & ADVERTISE_10FULL) {
			np->speed = 10;
			np->full_duplex = 1;
			printk (KERN_INFO "Auto 10 Mbps, Full duplex\n");
		} else if (negotiate & ADVERTISE_10HALF) {
			np->speed = 10;
			np->full_duplex = 0;
			printk (KERN_INFO "Auto 10 Mbps, Half duplex\n");
		}
		if (negotiate & ADVERTISE_PAUSE_CAP) {
			np->tx_flow &= 1;
			np->rx_flow &= 1;
		} else if (negotiate & ADVERTISE_PAUSE_ASYM) {
			np->tx_flow = 0;
			np->rx_flow &= 1;
		}
		/* else tx_flow, rx_flow = user select  */
	} else {
		__u16 bmcr = mii_read (dev, phy_addr, MII_BMCR);
		switch (bmcr & (BMCR_SPEED100 | BMCR_SPEED1000)) {
		case BMCR_SPEED1000:
			printk (KERN_INFO "Operating at 1000 Mbps, ");
			break;
		case BMCR_SPEED100:
			printk (KERN_INFO "Operating at 100 Mbps, ");
			break;
		case 0:
			printk (KERN_INFO "Operating at 10 Mbps, ");
		}
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
