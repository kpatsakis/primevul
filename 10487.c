static void rtrs_clt_hb_err_handler(struct rtrs_con *c)
{
	struct rtrs_clt_con *con = container_of(c, typeof(*con), c);

	rtrs_rdma_error_recovery(con);
}