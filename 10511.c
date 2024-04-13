static inline void path_it_init(struct path_it *it, struct rtrs_clt_sess *clt)
{
	INIT_LIST_HEAD(&it->skip_list);
	it->clt = clt;
	it->i = 0;

	if (clt->mp_policy == MP_POLICY_RR)
		it->next_path = get_next_path_rr;
	else if (clt->mp_policy == MP_POLICY_MIN_INFLIGHT)
		it->next_path = get_next_path_min_inflight;
	else
		it->next_path = get_next_path_min_latency;
}