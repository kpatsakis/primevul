mii_wait_link (struct net_device *dev, int wait)
{
	__u16 bmsr;
	int phy_addr;
	struct netdev_private *np;

	np = netdev_priv(dev);
	phy_addr = np->phy_addr;

	do {
		bmsr = mii_read (dev, phy_addr, MII_BMSR);
		if (bmsr & BMSR_LSTATUS)
			return 0;
		mdelay (1);
	} while (--wait > 0);
	return -1;
}
