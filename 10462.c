static int rtrs_post_rdma_write_sg(struct rtrs_clt_con *con,
				   struct rtrs_clt_io_req *req,
				   struct rtrs_rbuf *rbuf, bool fr_en,
				   u32 size, u32 imm, struct ib_send_wr *wr,
				   struct ib_send_wr *tail)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);
	struct ib_sge *sge = req->sge;
	enum ib_send_flags flags;
	struct scatterlist *sg;
	size_t num_sge;
	int i;
	struct ib_send_wr *ptail = NULL;

	if (fr_en) {
		i = 0;
		sge[i].addr   = req->mr->iova;
		sge[i].length = req->mr->length;
		sge[i].lkey   = req->mr->lkey;
		i++;
		num_sge = 2;
		ptail = tail;
	} else {
		for_each_sg(req->sglist, sg, req->sg_cnt, i) {
			sge[i].addr   = sg_dma_address(sg);
			sge[i].length = sg_dma_len(sg);
			sge[i].lkey   = clt_path->s.dev->ib_pd->local_dma_lkey;
		}
		num_sge = 1 + req->sg_cnt;
	}
	sge[i].addr   = req->iu->dma_addr;
	sge[i].length = size;
	sge[i].lkey   = clt_path->s.dev->ib_pd->local_dma_lkey;

	/*
	 * From time to time we have to post signalled sends,
	 * or send queue will fill up and only QP reset can help.
	 */
	flags = atomic_inc_return(&con->c.wr_cnt) % clt_path->s.signal_interval ?
			0 : IB_SEND_SIGNALED;

	ib_dma_sync_single_for_device(clt_path->s.dev->ib_dev,
				      req->iu->dma_addr,
				      size, DMA_TO_DEVICE);

	return rtrs_iu_post_rdma_write_imm(&con->c, req->iu, sge, num_sge,
					    rbuf->rkey, rbuf->addr, imm,
					    flags, wr, ptail);
}