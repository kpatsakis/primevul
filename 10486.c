static void rtrs_clt_add_path_to_arr(struct rtrs_clt_path *clt_path)
{
	struct rtrs_clt_sess *clt = clt_path->clt;

	mutex_lock(&clt->paths_mutex);
	clt->paths_num++;

	list_add_tail_rcu(&clt_path->s.entry, &clt->paths_list);
	mutex_unlock(&clt->paths_mutex);
}