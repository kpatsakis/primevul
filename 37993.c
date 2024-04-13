static int orinoco_ioctl_commit(struct net_device *dev,
				struct iw_request_info *info,
				void *wrqu,
				char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	unsigned long flags;
	int err = 0;

	if (!priv->open)
		return 0;

	if (orinoco_lock(priv, &flags) != 0)
		return err;

	err = orinoco_commit(priv);

	orinoco_unlock(priv, &flags);
	return err;
}
