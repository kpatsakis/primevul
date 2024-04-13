mii_set_media (struct net_device *dev)
{
	__u16 pscr;
	__u16 bmcr;
	__u16 bmsr;
	__u16 anar;
	int phy_addr;
	struct netdev_private *np;
	np = netdev_priv(dev);
	phy_addr = np->phy_addr;

	/* Does user set speed? */
	if (np->an_enable) {
		/* Advertise capabilities */
		bmsr = mii_read (dev, phy_addr, MII_BMSR);
		anar = mii_read (dev, phy_addr, MII_ADVERTISE) &
			~(ADVERTISE_100FULL | ADVERTISE_10FULL |
			  ADVERTISE_100HALF | ADVERTISE_10HALF |
			  ADVERTISE_100BASE4);
		if (bmsr & BMSR_100FULL)
			anar |= ADVERTISE_100FULL;
		if (bmsr & BMSR_100HALF)
			anar |= ADVERTISE_100HALF;
		if (bmsr & BMSR_100BASE4)
			anar |= ADVERTISE_100BASE4;
		if (bmsr & BMSR_10FULL)
			anar |= ADVERTISE_10FULL;
		if (bmsr & BMSR_10HALF)
			anar |= ADVERTISE_10HALF;
		anar |= ADVERTISE_PAUSE_CAP | ADVERTISE_PAUSE_ASYM;
		mii_write (dev, phy_addr, MII_ADVERTISE, anar);

		/* Enable Auto crossover */
		pscr = mii_read (dev, phy_addr, MII_PHY_SCR);
		pscr |= 3 << 5;	/* 11'b */
		mii_write (dev, phy_addr, MII_PHY_SCR, pscr);

		/* Soft reset PHY */
		mii_write (dev, phy_addr, MII_BMCR, BMCR_RESET);
		bmcr = BMCR_ANENABLE | BMCR_ANRESTART | BMCR_RESET;
		mii_write (dev, phy_addr, MII_BMCR, bmcr);
		mdelay(1);
	} else {
		/* Force speed setting */
		/* 1) Disable Auto crossover */
		pscr = mii_read (dev, phy_addr, MII_PHY_SCR);
		pscr &= ~(3 << 5);
		mii_write (dev, phy_addr, MII_PHY_SCR, pscr);

		/* 2) PHY Reset */
		bmcr = mii_read (dev, phy_addr, MII_BMCR);
		bmcr |= BMCR_RESET;
		mii_write (dev, phy_addr, MII_BMCR, bmcr);

		/* 3) Power Down */
		bmcr = 0x1940;	/* must be 0x1940 */
		mii_write (dev, phy_addr, MII_BMCR, bmcr);
		mdelay (100);	/* wait a certain time */

		/* 4) Advertise nothing */
		mii_write (dev, phy_addr, MII_ADVERTISE, 0);

		/* 5) Set media and Power Up */
		bmcr = BMCR_PDOWN;
		if (np->speed == 100) {
			bmcr |= BMCR_SPEED100;
			printk (KERN_INFO "Manual 100 Mbps, ");
		} else if (np->speed == 10) {
			printk (KERN_INFO "Manual 10 Mbps, ");
		}
		if (np->full_duplex) {
			bmcr |= BMCR_FULLDPLX;
			printk (KERN_CONT "Full duplex\n");
		} else {
			printk (KERN_CONT "Half duplex\n");
		}
#if 0
		/* Set 1000BaseT Master/Slave setting */
		mscr = mii_read (dev, phy_addr, MII_CTRL1000);
		mscr |= MII_MSCR_CFG_ENABLE;
		mscr &= ~MII_MSCR_CFG_VALUE = 0;
#endif
		mii_write (dev, phy_addr, MII_BMCR, bmcr);
		mdelay(10);
	}
	return 0;
}
