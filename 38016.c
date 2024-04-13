static int orinoco_ioctl_setwap(struct net_device *dev,
				struct iw_request_info *info,
				struct sockaddr *ap_addr,
				char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	int err = -EINPROGRESS;		/* Call commit handler */
	unsigned long flags;
	static const u8 off_addr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	static const u8 any_addr[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	/* Enable automatic roaming - no sanity checks are needed */
	if (memcmp(&ap_addr->sa_data, off_addr, ETH_ALEN) == 0 ||
	    memcmp(&ap_addr->sa_data, any_addr, ETH_ALEN) == 0) {
		priv->bssid_fixed = 0;
		memset(priv->desired_bssid, 0, ETH_ALEN);

		/* "off" means keep existing connection */
		if (ap_addr->sa_data[0] == 0) {
			__orinoco_hw_set_wap(priv);
			err = 0;
		}
		goto out;
	}

	if (priv->firmware_type == FIRMWARE_TYPE_AGERE) {
		printk(KERN_WARNING "%s: Lucent/Agere firmware doesn't "
		       "support manual roaming\n",
		       dev->name);
		err = -EOPNOTSUPP;
		goto out;
	}

	if (priv->iw_mode != NL80211_IFTYPE_STATION) {
		printk(KERN_WARNING "%s: Manual roaming supported only in "
		       "managed mode\n", dev->name);
		err = -EOPNOTSUPP;
		goto out;
	}

	/* Intersil firmware hangs without Desired ESSID */
	if (priv->firmware_type == FIRMWARE_TYPE_INTERSIL &&
	    strlen(priv->desired_essid) == 0) {
		printk(KERN_WARNING "%s: Desired ESSID must be set for "
		       "manual roaming\n", dev->name);
		err = -EOPNOTSUPP;
		goto out;
	}

	/* Finally, enable manual roaming */
	priv->bssid_fixed = 1;
	memcpy(priv->desired_bssid, &ap_addr->sa_data, ETH_ALEN);

 out:
	orinoco_unlock(priv, &flags);
	return err;
}
