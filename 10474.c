static int rtrs_clt_failover_req(struct rtrs_clt_sess *clt,
				 struct rtrs_clt_io_req *fail_req)
{
	struct rtrs_clt_path *alive_path;
	struct rtrs_clt_io_req *req;
	int err = -ECONNABORTED;
	struct path_it it;

	rcu_read_lock();
	for (path_it_init(&it, clt);
	     (alive_path = it.next_path(&it)) && it.i < it.clt->paths_num;
	     it.i++) {
		if (READ_ONCE(alive_path->state) != RTRS_CLT_CONNECTED)
			continue;
		req = rtrs_clt_get_copy_req(alive_path, fail_req);
		if (req->dir == DMA_TO_DEVICE)
			err = rtrs_clt_write_req(req);
		else
			err = rtrs_clt_read_req(req);
		if (err) {
			req->in_use = false;
			continue;
		}
		/* Success path */
		rtrs_clt_inc_failover_cnt(alive_path->stats);
		break;
	}
	path_it_deinit(&it);
	rcu_read_unlock();

	return err;
}