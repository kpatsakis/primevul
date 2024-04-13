int wvlan_uil_send_diag_msg(struct uilreq *urq, struct wl_private *lp)
{
	int         result = 0;
	DESC_STRCT  Descp[1];
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_uil_send_diag_msg");
	DBG_ENTER(DbgInfo);

	if (urq->hcfCtx == &(lp->hcfCtx)) {
		if (capable(CAP_NET_ADMIN)) {
			if ((urq->data != NULL) && (urq->len != 0)) {
				if (lp->hcfCtx.IFB_RscInd != 0) {
					u_char *data;

					/* Verify the user buffer */
					result = verify_area(VERIFY_READ, urq->data, urq->len);
					if (result != 0) {
						DBG_ERROR(DbgInfo, "verify_area failed, result: %d\n", result);
						urq->result = UIL_FAILURE;
						DBG_LEAVE(DbgInfo);
						return result;
					}

					data = kmalloc(urq->len, GFP_KERNEL);
					if (data != NULL) {
						memset(Descp, 0, sizeof(DESC_STRCT));
						memcpy(data, urq->data, urq->len);

						Descp[0].buf_addr       = (wci_bufp)data;
						Descp[0].BUF_CNT        = urq->len;
						Descp[0].next_desc_addr = 0;    /* terminate list */

						hcf_send_msg(&(lp->hcfCtx),  &Descp[0], HCF_PORT_0);
						kfree(data);
					} else {
						DBG_ERROR(DbgInfo, "ENOMEM\n");
						urq->result = UIL_FAILURE;
						result = -ENOMEM;
						DBG_LEAVE(DbgInfo);
						return result;
					}

				} else {
					urq->result = UIL_ERR_BUSY;
				}

			} else {
				urq->result = UIL_FAILURE;
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
} /* wvlan_uil_send_diag_msg */
