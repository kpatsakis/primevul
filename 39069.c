int wvlan_uil_connect(struct uilreq *urq, struct wl_private *lp)
{
	int result = 0;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_uil_connect");
	DBG_ENTER(DbgInfo);


	if (!(lp->flags & WVLAN2_UIL_CONNECTED)) {
		lp->flags |= WVLAN2_UIL_CONNECTED;
		urq->hcfCtx = &(lp->hcfCtx);
		urq->result = UIL_SUCCESS;
	} else {
		DBG_WARNING(DbgInfo, "UIL_ERR_IN_USE\n");
		urq->result = UIL_ERR_IN_USE;
	}

	DBG_LEAVE(DbgInfo);
	return result;
} /* wvlan_uil_connect */
