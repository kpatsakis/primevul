int wvlan_uil_disconnect(struct uilreq *urq, struct wl_private *lp)
{
	int result = 0;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_uil_disconnect");
	DBG_ENTER(DbgInfo);


	if (urq->hcfCtx == &(lp->hcfCtx)) {
		if (lp->flags & WVLAN2_UIL_CONNECTED) {
			lp->flags &= ~WVLAN2_UIL_CONNECTED;
			/*
			if (lp->flags & WVLAN2_UIL_BUSY) {
				lp->flags &= ~WVLAN2_UIL_BUSY;
				netif_start_queue(lp->dev);
			}
			*/
		}

		urq->hcfCtx = NULL;
		urq->result = UIL_SUCCESS;
	} else {
		DBG_ERROR(DbgInfo, "UIL_ERR_WRONG_IFB\n");
		urq->result = UIL_ERR_WRONG_IFB;
	}

	DBG_LEAVE(DbgInfo);
	return result;
} /* wvlan_uil_disconnect */
