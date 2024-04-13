static int orinoco_ioctl_get_encodeext(struct net_device *dev,
				       struct iw_request_info *info,
				       union iwreq_data *wrqu,
				       char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	struct iw_point *encoding = &wrqu->encoding;
	struct iw_encode_ext *ext = (struct iw_encode_ext *)extra;
	int idx, max_key_len;
	unsigned long flags;
	int err;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	err = -EINVAL;
	max_key_len = encoding->length - sizeof(*ext);
	if (max_key_len < 0)
		goto out;

	idx = encoding->flags & IW_ENCODE_INDEX;
	if (idx) {
		if ((idx < 1) || (idx > 4))
			goto out;
		idx--;
	} else
		idx = priv->tx_key;

	encoding->flags = idx + 1;
	memset(ext, 0, sizeof(*ext));

	switch (priv->encode_alg) {
	case ORINOCO_ALG_NONE:
		ext->alg = IW_ENCODE_ALG_NONE;
		ext->key_len = 0;
		encoding->flags |= IW_ENCODE_DISABLED;
		break;
	case ORINOCO_ALG_WEP:
		ext->alg = IW_ENCODE_ALG_WEP;
		ext->key_len = min(priv->keys[idx].key_len, max_key_len);
		memcpy(ext->key, priv->keys[idx].key, ext->key_len);
		encoding->flags |= IW_ENCODE_ENABLED;
		break;
	case ORINOCO_ALG_TKIP:
		ext->alg = IW_ENCODE_ALG_TKIP;
		ext->key_len = min(priv->keys[idx].key_len, max_key_len);
		memcpy(ext->key, priv->keys[idx].key, ext->key_len);
		encoding->flags |= IW_ENCODE_ENABLED;
		break;
	}

	err = 0;
 out:
	orinoco_unlock(priv, &flags);

	return err;
}
