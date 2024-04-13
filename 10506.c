static void complete_rdma_req(struct rtrs_clt_io_req *req, int errno,
			      bool notify, bool can_wait)
{
	struct rtrs_clt_con *con = req->con;
	struct rtrs_clt_path *clt_path;
	int err;

	if (WARN_ON(!req->in_use))
		return;
	if (WARN_ON(!req->con))
		return;
	clt_path = to_clt_path(con->c.path);

	if (req->sg_cnt) {
		if (req->dir == DMA_FROM_DEVICE && req->need_inv) {
			/*
			 * We are here to invalidate read requests
			 * ourselves.  In normal scenario server should
			 * send INV for all read requests, but
			 * we are here, thus two things could happen:
			 *
			 *    1.  this is failover, when errno != 0
			 *        and can_wait == 1,
			 *
			 *    2.  something totally bad happened and
			 *        server forgot to send INV, so we
			 *        should do that ourselves.
			 */

			if (can_wait) {
				req->need_inv_comp = true;
			} else {
				/* This should be IO path, so always notify */
				WARN_ON(!notify);
				/* Save errno for INV callback */
				req->inv_errno = errno;
			}

			refcount_inc(&req->ref);
			err = rtrs_inv_rkey(req);
			if (err) {
				rtrs_err(con->c.path, "Send INV WR key=%#x: %d\n",
					  req->mr->rkey, err);
			} else if (can_wait) {
				wait_for_completion(&req->inv_comp);
			} else {
				/*
				 * Something went wrong, so request will be
				 * completed from INV callback.
				 */
				WARN_ON_ONCE(1);

				return;
			}
			if (!refcount_dec_and_test(&req->ref))
				return;
		}
		ib_dma_unmap_sg(clt_path->s.dev->ib_dev, req->sglist,
				req->sg_cnt, req->dir);
	}
	if (!refcount_dec_and_test(&req->ref))
		return;
	if (req->mp_policy == MP_POLICY_MIN_INFLIGHT)
		atomic_dec(&clt_path->stats->inflight);

	req->in_use = false;
	req->con = NULL;

	if (errno) {
		rtrs_err_rl(con->c.path, "IO request failed: error=%d path=%s [%s:%u] notify=%d\n",
			    errno, kobject_name(&clt_path->kobj), clt_path->hca_name,
			    clt_path->hca_port, notify);
	}

	if (notify)
		req->conf(req->priv, errno);
}