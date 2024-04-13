static int rtrs_inv_rkey(struct rtrs_clt_io_req *req)
{
	struct rtrs_clt_con *con = req->con;
	struct ib_send_wr wr = {
		.opcode		    = IB_WR_LOCAL_INV,
		.wr_cqe		    = &req->inv_cqe,
		.send_flags	    = IB_SEND_SIGNALED,
		.ex.invalidate_rkey = req->mr->rkey,
	};
	req->inv_cqe.done = rtrs_clt_inv_rkey_done;

	return ib_post_send(con->c.qp, &wr, NULL);
}