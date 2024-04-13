static inline bool rtrs_clt_is_connected(const struct rtrs_clt_sess *clt)
{
	struct rtrs_clt_path *clt_path;
	bool connected = false;

	rcu_read_lock();
	list_for_each_entry_rcu(clt_path, &clt->paths_list, s.entry)
		connected |= READ_ONCE(clt_path->state) == RTRS_CLT_CONNECTED;
	rcu_read_unlock();

	return connected;
}