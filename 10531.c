int rtrs_clt_remove_path_from_sysfs(struct rtrs_clt_path *clt_path,
				     const struct attribute *sysfs_self)
{
	enum rtrs_clt_state old_state;
	bool changed;

	/*
	 * Continue stopping path till state was changed to DEAD or
	 * state was observed as DEAD:
	 * 1. State was changed to DEAD - we were fast and nobody
	 *    invoked rtrs_clt_reconnect(), which can again start
	 *    reconnecting.
	 * 2. State was observed as DEAD - we have someone in parallel
	 *    removing the path.
	 */
	do {
		rtrs_clt_close_conns(clt_path, true);
		changed = rtrs_clt_change_state_get_old(clt_path,
							RTRS_CLT_DEAD,
							&old_state);
	} while (!changed && old_state != RTRS_CLT_DEAD);

	if (changed) {
		rtrs_clt_remove_path_from_arr(clt_path);
		rtrs_clt_destroy_path_files(clt_path, sysfs_self);
		kobject_put(&clt_path->kobj);
	}

	return 0;
}