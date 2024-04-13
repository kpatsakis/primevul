int wvlan_uil_unblock(struct uilreq *urq, struct wl_private *lp)
{
	int result = 0;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_uil_unblock");
	DBG_ENTER(DbgInfo);

	if (urq->hcfCtx == &(lp->hcfCtx)) {
		if (capable(CAP_NET_ADMIN)) {
			if (lp->flags & WVLAN2_UIL_BUSY) {
				lp->flags &= ~WVLAN2_UIL_BUSY;
				netif_wake_queue(lp->dev);
				WL_WDS_NETIF_WAKE_QUEUE(lp);
			}
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
} /* wvlan_uil_unblock */
