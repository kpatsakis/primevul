static int rtrs_rdma_addr_resolved(struct rtrs_clt_con *con)
{
	struct rtrs_path *s = con->c.path;
	int err;

	mutex_lock(&con->con_mutex);
	err = create_con_cq_qp(con);
	mutex_unlock(&con->con_mutex);
	if (err) {
		rtrs_err(s, "create_con_cq_qp(), err: %d\n", err);
		return err;
	}
	err = rdma_resolve_route(con->c.cm_id, RTRS_CONNECT_TIMEOUT_MS);
	if (err)
		rtrs_err(s, "Resolving route failed, err: %d\n", err);

	return err;
}