static inline void flag_success_on_conn(struct rtrs_clt_con *con)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);

	atomic_inc(&clt_path->connected_cnt);
	con->cm_err = 1;
}