int wvlan_set_porttype(struct net_device *dev,
		      struct iw_request_info *info,
		      union iwreq_data *wrqu,
		      char *extra)
{
	struct wl_private *lp = wl_priv(dev);
	unsigned long flags;
	int     ret = 0;
	hcf_16  portType;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_set_porttype");
	DBG_ENTER(DbgInfo);

	wl_lock(lp, &flags);

	/* Validate the new value */
	portType = *((__u32 *)extra);

	if (!((portType == 1) || (portType == 3))) {
		ret = -EINVAL;
		goto out_unlock;
	}

	lp->PortType = portType;

	/* Commit the adapter parameters */
	wl_apply(lp);

out_unlock:
	wl_unlock(lp, &flags);

/* out: */
	DBG_LEAVE(DbgInfo);
	return ret;
}
