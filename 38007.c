static int orinoco_ioctl_set_genie(struct net_device *dev,
				   struct iw_request_info *info,
				   union iwreq_data *wrqu, char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	u8 *buf;
	unsigned long flags;

	/* cut off at IEEE80211_MAX_DATA_LEN */
	if ((wrqu->data.length > IEEE80211_MAX_DATA_LEN) ||
	    (wrqu->data.length && (extra == NULL)))
		return -EINVAL;

	if (wrqu->data.length) {
		buf = kmemdup(extra, wrqu->data.length, GFP_KERNEL);
		if (buf == NULL)
			return -ENOMEM;
	} else
		buf = NULL;

	if (orinoco_lock(priv, &flags) != 0) {
		kfree(buf);
		return -EBUSY;
	}

	kfree(priv->wpa_ie);
	priv->wpa_ie = buf;
	priv->wpa_ie_len = wrqu->data.length;

	if (priv->wpa_ie) {
		/* Looks like wl_lkm wants to check the auth alg, and
		 * somehow pass it to the firmware.
		 * Instead it just calls the key mgmt rid
		 *   - we do this in set auth.
		 */
	}

	orinoco_unlock(priv, &flags);
	return 0;
}
