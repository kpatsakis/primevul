int cfg_driver_identity(struct uilreq *urq, struct wl_private *lp)
{
	int result = 0;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_driver_identity");
	DBG_ENTER(DbgInfo);


	/* Make sure that user buffer can handle the driver identity structure. */
	if (urq->len < sizeof(lp->driverIdentity)) {
		urq->len = sizeof(lp->driverIdentity);
		urq->result = UIL_ERR_LEN;
		DBG_LEAVE(DbgInfo);
		return result;
	}

	/* Verify the user buffer. */
	result = verify_area(VERIFY_WRITE, urq->data, sizeof(lp->driverIdentity));
	if (result != 0) {
		urq->result = UIL_FAILURE;
		DBG_LEAVE(DbgInfo);
		return result;
	}

	/* Copy the driver identity into the user's buffer. */
	urq->result = UIL_SUCCESS;
	copy_to_user(urq->data, &(lp->driverIdentity), sizeof(lp->driverIdentity));

	DBG_LEAVE(DbgInfo);
	return result;
} /* cfg_driver_identity */
