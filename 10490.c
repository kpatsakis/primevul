int rtrs_clt_create_path_from_sysfs(struct rtrs_clt_sess *clt,
				     struct rtrs_addr *addr)
{
	struct rtrs_clt_path *clt_path;
	int err;

	clt_path = alloc_path(clt, addr, nr_cpu_ids, 0);
	if (IS_ERR(clt_path))
		return PTR_ERR(clt_path);

	mutex_lock(&clt->paths_mutex);
	if (clt->paths_num == 0) {
		/*
		 * When all the paths are removed for a session,
		 * the addition of the first path is like a new session for
		 * the storage server
		 */
		clt_path->for_new_clt = 1;
	}

	mutex_unlock(&clt->paths_mutex);

	/*
	 * It is totally safe to add path in CONNECTING state: coming
	 * IO will never grab it.  Also it is very important to add
	 * path before init, since init fires LINK_CONNECTED event.
	 */
	rtrs_clt_add_path_to_arr(clt_path);

	err = init_path(clt_path);
	if (err)
		goto close_path;

	err = rtrs_clt_create_path_files(clt_path);
	if (err)
		goto close_path;

	return 0;

close_path:
	rtrs_clt_remove_path_from_arr(clt_path);
	rtrs_clt_close_conns(clt_path, true);
	free_percpu(clt_path->stats->pcpu_stats);
	kfree(clt_path->stats);
	free_path(clt_path);

	return err;
}