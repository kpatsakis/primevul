static void free_permits(struct rtrs_clt_sess *clt)
{
	if (clt->permits_map) {
		size_t sz = clt->queue_depth;

		wait_event(clt->permits_wait,
			   find_first_bit(clt->permits_map, sz) >= sz);
	}
	kfree(clt->permits_map);
	clt->permits_map = NULL;
	kfree(clt->permits);
	clt->permits = NULL;
}