static int create_con_cq_qp(struct rtrs_clt_con *con)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	u32 max_send_wr, max_recv_wr, cq_num, max_send_sge, wr_limit;
	int err, cq_vector;
	struct rtrs_msg_rkey_rsp *rsp;

	lockdep_assert_held(&con->con_mutex);
	if (con->c.cid == 0) {
		max_send_sge = 1;
		/* We must be the first here */
		if (WARN_ON(clt_path->s.dev))
			return -EINVAL;

		/*
		 * The whole session uses device from user connection.
		 * Be careful not to close user connection before ib dev
		 * is gracefully put.
		 */
		clt_path->s.dev = rtrs_ib_dev_find_or_add(con->c.cm_id->device,
						       &dev_pd);
		if (!clt_path->s.dev) {
			rtrs_wrn(clt_path->clt,
				  "rtrs_ib_dev_find_get_or_add(): no memory\n");
			return -ENOMEM;
		}
		clt_path->s.dev_ref = 1;
		query_fast_reg_mode(clt_path);
		wr_limit = clt_path->s.dev->ib_dev->attrs.max_qp_wr;
		/*
		 * Two (request + registration) completion for send
		 * Two for recv if always_invalidate is set on server
		 * or one for recv.
		 * + 2 for drain and heartbeat
		 * in case qp gets into error state.
		 */
		max_send_wr =
			min_t(int, wr_limit, SERVICE_CON_QUEUE_DEPTH * 2 + 2);
		max_recv_wr = max_send_wr;
	} else {
		/*
		 * Here we assume that session members are correctly set.
		 * This is always true if user connection (cid == 0) is
		 * established first.
		 */
		if (WARN_ON(!clt_path->s.dev))
			return -EINVAL;
		if (WARN_ON(!clt_path->queue_depth))
			return -EINVAL;

		wr_limit = clt_path->s.dev->ib_dev->attrs.max_qp_wr;
		/* Shared between connections */
		clt_path->s.dev_ref++;
		max_send_wr = min_t(int, wr_limit,
			      /* QD * (REQ + RSP + FR REGS or INVS) + drain */
			      clt_path->queue_depth * 3 + 1);
		max_recv_wr = min_t(int, wr_limit,
			      clt_path->queue_depth * 3 + 1);
		max_send_sge = 2;
	}
	atomic_set(&con->c.sq_wr_avail, max_send_wr);
	cq_num = max_send_wr + max_recv_wr;
	/* alloc iu to recv new rkey reply when server reports flags set */
	if (clt_path->flags & RTRS_MSG_NEW_RKEY_F || con->c.cid == 0) {
		con->rsp_ius = rtrs_iu_alloc(cq_num, sizeof(*rsp),
					      GFP_KERNEL,
					      clt_path->s.dev->ib_dev,
					      DMA_FROM_DEVICE,
					      rtrs_clt_rdma_done);
		if (!con->rsp_ius)
			return -ENOMEM;
		con->queue_num = cq_num;
	}
	cq_num = max_send_wr + max_recv_wr;
	cq_vector = con->cpu % clt_path->s.dev->ib_dev->num_comp_vectors;
	if (con->c.cid >= clt_path->s.irq_con_num)
		err = rtrs_cq_qp_create(&clt_path->s, &con->c, max_send_sge,
					cq_vector, cq_num, max_send_wr,
					max_recv_wr, IB_POLL_DIRECT);
	else
		err = rtrs_cq_qp_create(&clt_path->s, &con->c, max_send_sge,
					cq_vector, cq_num, max_send_wr,
					max_recv_wr, IB_POLL_SOFTIRQ);
	/*
	 * In case of error we do not bother to clean previous allocations,
	 * since destroy_con_cq_qp() must be called.
	 */
	return err;
}