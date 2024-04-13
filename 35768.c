change_mtu (struct net_device *dev, int new_mtu)
{
	struct netdev_private *np = netdev_priv(dev);
	int max = (np->jumbo) ? MAX_JUMBO : 1536;

	if ((new_mtu < 68) || (new_mtu > max)) {
		return -EINVAL;
	}

	dev->mtu = new_mtu;

	return 0;
}
