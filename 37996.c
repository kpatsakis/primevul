static int orinoco_ioctl_get_genie(struct net_device *dev,
				   struct iw_request_info *info,
				   union iwreq_data *wrqu, char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	unsigned long flags;
	int err = 0;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	if ((priv->wpa_ie_len == 0) || (priv->wpa_ie == NULL)) {
		wrqu->data.length = 0;
		goto out;
	}

	if (wrqu->data.length < priv->wpa_ie_len) {
		err = -E2BIG;
		goto out;
	}

	wrqu->data.length = priv->wpa_ie_len;
	memcpy(extra, priv->wpa_ie, priv->wpa_ie_len);

out:
	orinoco_unlock(priv, &flags);
	return err;
}
