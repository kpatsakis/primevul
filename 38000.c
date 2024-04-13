static int orinoco_ioctl_getport3(struct net_device *dev,
				  struct iw_request_info *info,
				  void *wrqu,
				  char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	int *val = (int *) extra;

	*val = priv->prefer_port3;
	return 0;
}
