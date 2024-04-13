int wvlan_uil_block(struct uilreq *urq, struct wl_private *lp)
{
	int result = 0;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_uil_block");
	DBG_ENTER(DbgInfo);

	if (urq->hcfCtx == &(lp->hcfCtx)) {
		if (capable(CAP_NET_ADMIN)) {
			lp->flags |= WVLAN2_UIL_BUSY;
			netif_stop_queue(lp->dev);
			WL_WDS_NETIF_STOP_QUEUE(lp);
			urq->result = UIL_SUCCESS;
		} else {
			DBG_ERROR(DbgInfo, "EPERM\n");
			urq->result = UIL_FAILURE;
			result = -EPERM;
		}
	} else {
		DBG_ERROR(DbgInfo, "UIL_ERR_WRONG_IFB\n");
		urq->result = UIL_ERR_WRONG_IFB;
	}

	DBG_LEAVE(DbgInfo);
	return result;
} /* wvlan_uil_block */
