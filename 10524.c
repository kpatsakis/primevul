static inline void flag_error_on_conn(struct rtrs_clt_con *con, int cm_err)
{
	if (con->cm_err == 1) {
		struct rtrs_clt_path *clt_path;

		clt_path = to_clt_path(con->c.path);
		if (atomic_dec_and_test(&clt_path->connected_cnt))

			wake_up(&clt_path->state_wq);
	}
	con->cm_err = cm_err;
}