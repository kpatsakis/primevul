int rtrs_clt_rdma_cq_direct(struct rtrs_clt_sess *clt, unsigned int index)
{
	/* If no path, return -1 for block layer not to try again */
	int cnt = -1;
	struct rtrs_con *con;
	struct rtrs_clt_path *clt_path;
	struct path_it it;

	rcu_read_lock();
	for (path_it_init(&it, clt);
	     (clt_path = it.next_path(&it)) && it.i < it.clt->paths_num; it.i++) {
		if (READ_ONCE(clt_path->state) != RTRS_CLT_CONNECTED)
			continue;

		con = clt_path->s.con[index + 1];
		cnt = ib_process_cq_direct(con->cq, -1);
		if (cnt)
			break;
	}
	path_it_deinit(&it);
	rcu_read_unlock();

	return cnt;
}