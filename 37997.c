static int orinoco_ioctl_getessid(struct net_device *dev,
				  struct iw_request_info *info,
				  struct iw_point *erq,
				  char *essidbuf)
{
	struct orinoco_private *priv = ndev_priv(dev);
	int active;
	int err = 0;
	unsigned long flags;

	if (netif_running(dev)) {
		err = orinoco_hw_get_essid(priv, &active, essidbuf);
		if (err < 0)
			return err;
		erq->length = err;
	} else {
		if (orinoco_lock(priv, &flags) != 0)
			return -EBUSY;
		memcpy(essidbuf, priv->desired_essid, IW_ESSID_MAX_SIZE);
		erq->length = strlen(priv->desired_essid);
		orinoco_unlock(priv, &flags);
	}

	erq->flags = 1;

	return 0;
}
