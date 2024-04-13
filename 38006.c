static int orinoco_ioctl_set_encodeext(struct net_device *dev,
				       struct iw_request_info *info,
				       union iwreq_data *wrqu,
				       char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	struct iw_point *encoding = &wrqu->encoding;
	struct iw_encode_ext *ext = (struct iw_encode_ext *)extra;
	int idx, alg = ext->alg, set_key = 1;
	unsigned long flags;
	int err = -EINVAL;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	/* Determine and validate the key index */
	idx = encoding->flags & IW_ENCODE_INDEX;
	if (idx) {
		if ((idx < 1) || (idx > 4))
			goto out;
		idx--;
	} else
		idx = priv->tx_key;

	if (encoding->flags & IW_ENCODE_DISABLED)
		alg = IW_ENCODE_ALG_NONE;

	if (priv->has_wpa && (alg != IW_ENCODE_ALG_TKIP)) {
		/* Clear any TKIP TX key we had */
		(void) orinoco_clear_tkip_key(priv, priv->tx_key);
	}

	if (ext->ext_flags & IW_ENCODE_EXT_SET_TX_KEY) {
		priv->tx_key = idx;
		set_key = ((alg == IW_ENCODE_ALG_TKIP) ||
			   (ext->key_len > 0)) ? 1 : 0;
	}

	if (set_key) {
		/* Set the requested key first */
		switch (alg) {
		case IW_ENCODE_ALG_NONE:
			priv->encode_alg = ORINOCO_ALG_NONE;
			err = orinoco_set_key(priv, idx, ORINOCO_ALG_NONE,
					      NULL, 0, NULL, 0);
			break;

		case IW_ENCODE_ALG_WEP:
			if (ext->key_len <= 0)
				goto out;

			priv->encode_alg = ORINOCO_ALG_WEP;
			err = orinoco_set_key(priv, idx, ORINOCO_ALG_WEP,
					      ext->key, ext->key_len, NULL, 0);
			break;

		case IW_ENCODE_ALG_TKIP:
		{
			u8 *tkip_iv = NULL;

			if (!priv->has_wpa ||
			    (ext->key_len > sizeof(struct orinoco_tkip_key)))
				goto out;

			priv->encode_alg = ORINOCO_ALG_TKIP;

			if (ext->ext_flags & IW_ENCODE_EXT_RX_SEQ_VALID)
				tkip_iv = &ext->rx_seq[0];

			err = orinoco_set_key(priv, idx, ORINOCO_ALG_TKIP,
					      ext->key, ext->key_len, tkip_iv,
					      ORINOCO_SEQ_LEN);

			err = __orinoco_hw_set_tkip_key(priv, idx,
				 ext->ext_flags & IW_ENCODE_EXT_SET_TX_KEY,
				 priv->keys[idx].key,
				 tkip_iv, ORINOCO_SEQ_LEN, NULL, 0);
			if (err)
				printk(KERN_ERR "%s: Error %d setting TKIP key"
				       "\n", dev->name, err);

			goto out;
		}
		default:
			goto out;
		}
	}
	err = -EINPROGRESS;
 out:
	orinoco_unlock(priv, &flags);

	return err;
}
