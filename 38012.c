static int orinoco_ioctl_setport3(struct net_device *dev,
				  struct iw_request_info *info,
				  void *wrqu,
				  char *extra)
{
	struct orinoco_private *priv = ndev_priv(dev);
	int val = *((int *) extra);
	int err = 0;
	unsigned long flags;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	switch (val) {
	case 0: /* Try to do IEEE ad-hoc mode */
		if (!priv->has_ibss) {
			err = -EINVAL;
			break;
		}
		priv->prefer_port3 = 0;

		break;

	case 1: /* Try to do Lucent proprietary ad-hoc mode */
		if (!priv->has_port3) {
			err = -EINVAL;
			break;
		}
		priv->prefer_port3 = 1;
		break;

	default:
		err = -EINVAL;
	}

	if (!err) {
		/* Actually update the mode we are using */
		set_port_type(priv);
		err = -EINPROGRESS;
	}

	orinoco_unlock(priv, &flags);

	return err;
}
