static int create_con(struct rtrs_clt_path *clt_path, unsigned int cid)
{
	struct rtrs_clt_con *con;

	con = kzalloc(sizeof(*con), GFP_KERNEL);
	if (!con)
		return -ENOMEM;

	/* Map first two connections to the first CPU */
	con->cpu  = (cid ? cid - 1 : 0) % nr_cpu_ids;
	con->c.cid = cid;
	con->c.path = &clt_path->s;
	/* Align with srv, init as 1 */
	atomic_set(&con->c.wr_cnt, 1);
	mutex_init(&con->con_mutex);

	clt_path->s.con[cid] = &con->c;

	return 0;
}