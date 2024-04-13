struct rtrs_clt_sess *rtrs_clt_open(struct rtrs_clt_ops *ops,
				 const char *pathname,
				 const struct rtrs_addr *paths,
				 size_t paths_num, u16 port,
				 size_t pdu_sz, u8 reconnect_delay_sec,
				 s16 max_reconnect_attempts, u32 nr_poll_queues)
{
	struct rtrs_clt_path *clt_path, *tmp;
	struct rtrs_clt_sess *clt;
	int err, i;

	if (strchr(pathname, '/') || strchr(pathname, '.')) {
		pr_err("pathname cannot contain / and .\n");
		err = -EINVAL;
		goto out;
	}

	clt = alloc_clt(pathname, paths_num, port, pdu_sz, ops->priv,
			ops->link_ev,
			reconnect_delay_sec,
			max_reconnect_attempts);
	if (IS_ERR(clt)) {
		err = PTR_ERR(clt);
		goto out;
	}
	for (i = 0; i < paths_num; i++) {
		struct rtrs_clt_path *clt_path;

		clt_path = alloc_path(clt, &paths[i], nr_cpu_ids,
				  nr_poll_queues);
		if (IS_ERR(clt_path)) {
			err = PTR_ERR(clt_path);
			goto close_all_path;
		}
		if (!i)
			clt_path->for_new_clt = 1;
		list_add_tail_rcu(&clt_path->s.entry, &clt->paths_list);

		err = init_path(clt_path);
		if (err) {
			list_del_rcu(&clt_path->s.entry);
			rtrs_clt_close_conns(clt_path, true);
			free_percpu(clt_path->stats->pcpu_stats);
			kfree(clt_path->stats);
			free_path(clt_path);
			goto close_all_path;
		}

		err = rtrs_clt_create_path_files(clt_path);
		if (err) {
			list_del_rcu(&clt_path->s.entry);
			rtrs_clt_close_conns(clt_path, true);
			free_percpu(clt_path->stats->pcpu_stats);
			kfree(clt_path->stats);
			free_path(clt_path);
			goto close_all_path;
		}
	}
	err = alloc_permits(clt);
	if (err)
		goto close_all_path;

	return clt;

close_all_path:
	list_for_each_entry_safe(clt_path, tmp, &clt->paths_list, s.entry) {
		rtrs_clt_destroy_path_files(clt_path, NULL);
		rtrs_clt_close_conns(clt_path, true);
		kobject_put(&clt_path->kobj);
	}
	rtrs_clt_destroy_sysfs_root(clt);
	free_clt(clt);

out:
	return ERR_PTR(err);
}