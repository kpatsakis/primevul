static void stop_cm(struct rtrs_clt_con *con)
{
	rdma_disconnect(con->c.cm_id);
	if (con->c.qp)
		ib_drain_qp(con->c.qp);
}