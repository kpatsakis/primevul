int wvlan_get_porttype(struct net_device *dev,
		      struct iw_request_info *info,
		      union iwreq_data *wrqu,
		      char *extra)
{
	struct wl_private *lp = wl_priv(dev);
	unsigned long flags;
	int     ret = 0;
	int     status = -1;
	hcf_16  *pPortType;
	__u32 *pData = (__u32 *)extra;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_get_porttype");
	DBG_ENTER(DbgInfo);

	wl_lock(lp, &flags);

	/* Get the current port type */
	lp->ltvRecord.len = 1 + (sizeof(*pPortType) / sizeof(hcf_16));
	lp->ltvRecord.typ = CFG_CNF_PORT_TYPE;

	status = hcf_get_info(&(lp->hcfCtx), (LTVP)&(lp->ltvRecord));

	if (status == HCF_SUCCESS) {
		pPortType = (hcf_16 *)&(lp->ltvRecord.u.u32);

		*pData = CNV_LITTLE_TO_INT(*pPortType);
	} else {
	    ret = -EFAULT;
	}

	wl_unlock(lp, &flags);

/* out: */
	DBG_LEAVE(DbgInfo);
	return ret;
} /* wvlan_get_porttype */
