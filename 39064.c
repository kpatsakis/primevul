int wvlan_get_station_nickname(struct net_device *dev,
		      struct iw_request_info *info,
		      union iwreq_data *wrqu,
		      char *extra)
{
	struct wl_private *lp = wl_priv(dev);
	unsigned long flags;
	int         ret = 0;
	int         status = -1;
	wvName_t   *pName;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_get_station_nickname");
	DBG_ENTER(DbgInfo);

	wl_lock(lp, &flags);

	/* Get the current station name */
	lp->ltvRecord.len = 1 + (sizeof(*pName) / sizeof(hcf_16));
	lp->ltvRecord.typ = CFG_CNF_OWN_NAME;

	status = hcf_get_info(&(lp->hcfCtx), (LTVP)&(lp->ltvRecord));

	if (status == HCF_SUCCESS) {
		pName = (wvName_t *)&(lp->ltvRecord.u.u32);

		memset(extra, '\0', HCF_MAX_NAME_LEN);
		wrqu->data.length = pName->length;
		memcpy(extra, pName->name, pName->length);
	} else {
		ret = -EFAULT;
	}

	wl_unlock(lp, &flags);

/* out: */
	DBG_LEAVE(DbgInfo);
	return ret;
} /* wvlan_get_station_nickname */
