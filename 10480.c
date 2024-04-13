static struct rtrs_clt_path *get_next_path_rr(struct path_it *it)
{
	struct rtrs_clt_path __rcu **ppcpu_path;
	struct rtrs_clt_path *path;
	struct rtrs_clt_sess *clt;

	clt = it->clt;

	/*
	 * Here we use two RCU objects: @paths_list and @pcpu_path
	 * pointer.  See rtrs_clt_remove_path_from_arr() for details
	 * how that is handled.
	 */

	ppcpu_path = this_cpu_ptr(clt->pcpu_path);
	path = rcu_dereference(*ppcpu_path);
	if (!path)
		path = list_first_or_null_rcu(&clt->paths_list,
					      typeof(*path), s.entry);
	else
		path = list_next_or_null_rr_rcu(&clt->paths_list,
						&path->s.entry,
						typeof(*path),
						s.entry);
	rcu_assign_pointer(*ppcpu_path, path);

	return path;
}