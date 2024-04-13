static void rtrs_clt_stop_and_destroy_conns(struct rtrs_clt_path *clt_path)
{
	struct rtrs_clt_con *con;
	unsigned int cid;

	WARN_ON(READ_ONCE(clt_path->state) == RTRS_CLT_CONNECTED);

	/*
	 * Possible race with rtrs_clt_open(), when DEVICE_REMOVAL comes
	 * exactly in between.  Start destroying after it finishes.
	 */
	mutex_lock(&clt_path->init_mutex);
	mutex_unlock(&clt_path->init_mutex);

	/*
	 * All IO paths must observe !CONNECTED state before we
	 * free everything.
	 */
	synchronize_rcu();

	rtrs_stop_hb(&clt_path->s);

	/*
	 * The order it utterly crucial: firstly disconnect and complete all
	 * rdma requests with error (thus set in_use=false for requests),
	 * then fail outstanding requests checking in_use for each, and
	 * eventually notify upper layer about session disconnection.
	 */

	for (cid = 0; cid < clt_path->s.con_num; cid++) {
		if (!clt_path->s.con[cid])
			break;
		con = to_clt_con(clt_path->s.con[cid]);
		stop_cm(con);
	}
	fail_all_outstanding_reqs(clt_path);
	free_path_reqs(clt_path);
	rtrs_clt_path_down(clt_path);

	/*
	 * Wait for graceful shutdown, namely when peer side invokes
	 * rdma_disconnect(). 'connected_cnt' is decremented only on
	 * CM events, thus if other side had crashed and hb has detected
	 * something is wrong, here we will stuck for exactly timeout ms,
	 * since CM does not fire anything.  That is fine, we are not in
	 * hurry.
	 */
	wait_event_timeout(clt_path->state_wq,
			   !atomic_read(&clt_path->connected_cnt),
			   msecs_to_jiffies(RTRS_CONNECT_TIMEOUT_MS));

	for (cid = 0; cid < clt_path->s.con_num; cid++) {
		if (!clt_path->s.con[cid])
			break;
		con = to_clt_con(clt_path->s.con[cid]);
		mutex_lock(&con->con_mutex);
		destroy_con_cq_qp(con);
		mutex_unlock(&con->con_mutex);
		destroy_cm(con);
		destroy_con(con);
	}
}