static int orinoco_ioctl_getrate(struct net_device *dev,
				 struct iw_request_info *info,
				 struct iw_param *rrq,
				 char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	int err = 0;
	int bitrate, automatic;
	unsigned long flags;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	orinoco_get_ratemode_cfg(priv->bitratemode, &bitrate, &automatic);

	/* If the interface is running we try to find more about the
	   current mode */
	if (netif_running(dev)) {
		int act_bitrate;
		int lerr;

		/* Ignore errors if we can't get the actual bitrate */
		lerr = orinoco_hw_get_act_bitrate(priv, &act_bitrate);
		if (!lerr)
			bitrate = act_bitrate;
	}

	orinoco_unlock(priv, &flags);

	rrq->value = bitrate;
	rrq->fixed = !automatic;
	rrq->disabled = 0;

	return err;
}
