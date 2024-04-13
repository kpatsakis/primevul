find_miiphy (struct net_device *dev)
{
	int i, phy_found = 0;
	struct netdev_private *np;
	long ioaddr;
	np = netdev_priv(dev);
	ioaddr = dev->base_addr;
	np->phy_addr = 1;

	for (i = 31; i >= 0; i--) {
		int mii_status = mii_read (dev, i, 1);
		if (mii_status != 0xffff && mii_status != 0x0000) {
			np->phy_addr = i;
			phy_found++;
		}
	}
	if (!phy_found) {
		printk (KERN_ERR "%s: No MII PHY found!\n", dev->name);
		return -ENODEV;
	}
	return 0;
}
