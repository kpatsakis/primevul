int cfg_driver_info(struct uilreq *urq, struct wl_private *lp)
{
	int result = 0;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("cfg_driver_info");
	DBG_ENTER(DbgInfo);


	/* Make sure that user buffer can handle the driver information buffer */
	if (urq->len < sizeof(lp->driverInfo)) {
		urq->len = sizeof(lp->driverInfo);
		urq->result = UIL_ERR_LEN;
		DBG_LEAVE(DbgInfo);
		return result;
	}

	/* Verify the user buffer. */
	result = verify_area(VERIFY_WRITE, urq->data, sizeof(lp->driverInfo));
	if (result != 0) {
		urq->result = UIL_FAILURE;
		DBG_LEAVE(DbgInfo);
		return result;
	}

	lp->driverInfo.card_stat = lp->hcfCtx.IFB_CardStat;

	/* Copy the driver information into the user's buffer. */
	urq->result = UIL_SUCCESS;
	copy_to_user(urq->data, &(lp->driverInfo), sizeof(lp->driverInfo));

	DBG_LEAVE(DbgInfo);
	return result;
} /* cfg_driver_info */
