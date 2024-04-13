static void destroy_con(struct rtrs_clt_con *con)
{
	struct rtrs_clt_path *clt_path = to_clt_path(con->c.path);

	clt_path->s.con[con->c.cid] = NULL;
	mutex_destroy(&con->con_mutex);
	kfree(con);
}