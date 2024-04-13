int rtrs_clt_query(struct rtrs_clt_sess *clt, struct rtrs_attrs *attr)
{
	if (!rtrs_clt_is_connected(clt))
		return -ECOMM;

	attr->queue_depth      = clt->queue_depth;
	attr->max_segments     = clt->max_segments;
	/* Cap max_io_size to min of remote buffer size and the fr pages */
	attr->max_io_size = min_t(int, clt->max_io_size,
				  clt->max_segments * SZ_4K);

	return 0;
}