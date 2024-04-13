static int orinoco_ioctl_get_auth(struct net_device *dev,
				  struct iw_request_info *info,
				  union iwreq_data *wrqu, char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	struct iw_param *param = &wrqu->param;
	unsigned long flags;
	int ret = 0;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	switch (param->flags & IW_AUTH_INDEX) {
	case IW_AUTH_KEY_MGMT:
		param->value = priv->key_mgmt;
		break;

	case IW_AUTH_TKIP_COUNTERMEASURES:
		param->value = priv->tkip_cm_active;
		break;

	case IW_AUTH_80211_AUTH_ALG:
		if (priv->wep_restrict)
			param->value = IW_AUTH_ALG_SHARED_KEY;
		else
			param->value = IW_AUTH_ALG_OPEN_SYSTEM;
		break;

	case IW_AUTH_WPA_ENABLED:
		param->value = priv->wpa_enabled;
		break;

	default:
		ret = -EOPNOTSUPP;
	}

	orinoco_unlock(priv, &flags);
	return ret;
}
