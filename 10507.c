static struct rtrs_clt_path *get_next_path_min_inflight(struct path_it *it)
{
	struct rtrs_clt_path *min_path = NULL;
	struct rtrs_clt_sess *clt = it->clt;
	struct rtrs_clt_path *clt_path;
	int min_inflight = INT_MAX;
	int inflight;

	list_for_each_entry_rcu(clt_path, &clt->paths_list, s.entry) {
		if (READ_ONCE(clt_path->state) != RTRS_CLT_CONNECTED)
			continue;

		if (!list_empty(raw_cpu_ptr(clt_path->mp_skip_entry)))
			continue;

		inflight = atomic_read(&clt_path->stats->inflight);

		if (inflight < min_inflight) {
			min_inflight = inflight;
			min_path = clt_path;
		}
	}

	/*
	 * add the path to the skip list, so that next time we can get
	 * a different one
	 */
	if (min_path)
		list_add(raw_cpu_ptr(min_path->mp_skip_entry), &it->skip_list);

	return min_path;
}