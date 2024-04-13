static int orinoco_ioctl_reset(struct net_device *dev,
			       struct iw_request_info *info,
			       void *wrqu,
			       char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (info->cmd == (SIOCIWFIRSTPRIV + 0x1)) {
		printk(KERN_DEBUG "%s: Forcing reset!\n", dev->name);

		/* Firmware reset */
		orinoco_reset(&priv->reset_work);
	} else {
		printk(KERN_DEBUG "%s: Force scheduling reset!\n", dev->name);

		schedule_work(&priv->reset_work);
	}

	return 0;
}
