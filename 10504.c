void rtrs_clt_put_permit(struct rtrs_clt_sess *clt,
			 struct rtrs_permit *permit)
{
	if (WARN_ON(!test_bit(permit->mem_id, clt->permits_map)))
		return;

	__rtrs_put_permit(clt, permit);

	/*
	 * rtrs_clt_get_permit() adds itself to the &clt->permits_wait list
	 * before calling schedule(). So if rtrs_clt_get_permit() is sleeping
	 * it must have added itself to &clt->permits_wait before
	 * __rtrs_put_permit() finished.
	 * Hence it is safe to guard wake_up() with a waitqueue_active() test.
	 */
	if (waitqueue_active(&clt->permits_wait))
		wake_up(&clt->permits_wait);
}