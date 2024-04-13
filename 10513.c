static void destroy_cm(struct rtrs_clt_con *con)
{
	rdma_destroy_id(con->c.cm_id);
	con->c.cm_id = NULL;
}