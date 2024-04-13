static u32 rio_get_link(struct net_device *dev)
{
	struct netdev_private *np = netdev_priv(dev);
	return np->link_status;
}
