static int orinoco_ioctl_setiwencode(struct net_device *dev,
				     struct iw_request_info *info,
				     struct iw_point *erq,
				     char *keybuf)
{
	struct orinoco_private *priv = ndev_priv(dev);
	int index = (erq->flags & IW_ENCODE_INDEX) - 1;
	int setindex = priv->tx_key;
	enum orinoco_alg encode_alg = priv->encode_alg;
	int restricted = priv->wep_restrict;
	int err = -EINPROGRESS;		/* Call commit handler */
	unsigned long flags;

	if (!priv->has_wep)
		return -EOPNOTSUPP;

	if (erq->pointer) {
		/* We actually have a key to set - check its length */
		if (erq->length > LARGE_KEY_SIZE)
			return -E2BIG;

		if ((erq->length > SMALL_KEY_SIZE) && !priv->has_big_wep)
			return -E2BIG;
	}

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	/* Clear any TKIP key we have */
	if ((priv->has_wpa) && (priv->encode_alg == ORINOCO_ALG_TKIP))
		(void) orinoco_clear_tkip_key(priv, setindex);

	if (erq->length > 0) {
		if ((index < 0) || (index >= ORINOCO_MAX_KEYS))
			index = priv->tx_key;

		/* Switch on WEP if off */
		if (encode_alg != ORINOCO_ALG_WEP) {
			setindex = index;
			encode_alg = ORINOCO_ALG_WEP;
		}
	} else {
		/* Important note : if the user do "iwconfig eth0 enc off",
		 * we will arrive there with an index of -1. This is valid
		 * but need to be taken care off... Jean II */
		if ((index < 0) || (index >= ORINOCO_MAX_KEYS)) {
			if ((index != -1) || (erq->flags == 0)) {
				err = -EINVAL;
				goto out;
			}
		} else {
			/* Set the index : Check that the key is valid */
			if (priv->keys[index].key_len == 0) {
				err = -EINVAL;
				goto out;
			}
			setindex = index;
		}
	}

	if (erq->flags & IW_ENCODE_DISABLED)
		encode_alg = ORINOCO_ALG_NONE;
	if (erq->flags & IW_ENCODE_OPEN)
		restricted = 0;
	if (erq->flags & IW_ENCODE_RESTRICTED)
		restricted = 1;

	if (erq->pointer && erq->length > 0) {
		err = orinoco_set_key(priv, index, ORINOCO_ALG_WEP, keybuf,
				      erq->length, NULL, 0);
	}
	priv->tx_key = setindex;

	/* Try fast key change if connected and only keys are changed */
	if ((priv->encode_alg == encode_alg) &&
	    (priv->wep_restrict == restricted) &&
	    netif_carrier_ok(dev)) {
		err = __orinoco_hw_setup_wepkeys(priv);
		/* No need to commit if successful */
		goto out;
	}

	priv->encode_alg = encode_alg;
	priv->wep_restrict = restricted;

 out:
	orinoco_unlock(priv, &flags);

	return err;
}
