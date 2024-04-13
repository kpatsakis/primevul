static void destroy_con_cq_qp(struct rtrs_clt_con *con)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);

	/*
	 * Be careful here: destroy_con_cq_qp() can be called even
	 * create_con_cq_qp() failed, see comments there.
	 */
	lockdep_assert_held(&con->con_mutex);
	rtrs_cq_qp_destroy(&con->c);
	if (con->rsp_ius) {
		rtrs_iu_free(con->rsp_ius, clt_path->s.dev->ib_dev,
			     con->queue_num);
		con->rsp_ius = NULL;
		con->queue_num = 0;
	}
	if (clt_path->s.dev_ref && !--clt_path->s.dev_ref) {
		rtrs_ib_dev_put(clt_path->s.dev);
		clt_path->s.dev = NULL;
	}
}