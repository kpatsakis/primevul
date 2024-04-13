static int orinoco_ioctl_setibssport(struct net_device *dev,
				     struct iw_request_info *info,
				     void *wrqu,
				     char *extra)

{
	struct orinoco_private *priv = ndev_priv(dev);
	int val = *((int *) extra);
	unsigned long flags;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	priv->ibss_port = val;

	/* Actually update the mode we are using */
	set_port_type(priv);

	orinoco_unlock(priv, &flags);
	return -EINPROGRESS;		/* Call commit handler */
}
