static int rtrs_post_send_rdma(struct rtrs_clt_con *con,
				struct rtrs_clt_io_req *req,
				struct rtrs_rbuf *rbuf, u32 off,
				u32 imm, struct ib_send_wr *wr)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	enum ib_send_flags flags;
	struct ib_sge sge;

	if (!req->sg_size) {
		rtrs_wrn(con->c.path,
			 "Doing RDMA Write failed, no data supplied\n");
		return -EINVAL;
	}

	/* user data and user message in the first list element */
	sge.addr   = req->iu->dma_addr;
	sge.length = req->sg_size;
	sge.lkey   = clt_path->s.dev->ib_pd->local_dma_lkey;

	/*
	 * From time to time we have to post signalled sends,
	 * or send queue will fill up and only QP reset can help.
	 */
	flags = atomic_inc_return(&con->c.wr_cnt) % clt_path->s.signal_interval ?
			0 : IB_SEND_SIGNALED;

	ib_dma_sync_single_for_device(clt_path->s.dev->ib_dev,
				      req->iu->dma_addr,
				      req->sg_size, DMA_TO_DEVICE);

	return rtrs_iu_post_rdma_write_imm(&con->c, req->iu, &sge, 1,
					    rbuf->rkey, rbuf->addr + off,
					    imm, flags, wr, NULL);
}