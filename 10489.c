static void query_fast_reg_mode(struct rtrs_clt_path *clt_path)
{
	struct ib_device *ib_dev;
	u64 max_pages_per_mr;
	int mr_page_shift;

	ib_dev = clt_path->s.dev->ib_dev;

	/*
	 * Use the smallest page size supported by the HCA, down to a
	 * minimum of 4096 bytes. We're unlikely to build large sglists
	 * out of smaller entries.
	 */
	mr_page_shift      = max(12, ffs(ib_dev->attrs.page_size_cap) - 1);
	max_pages_per_mr   = ib_dev->attrs.max_mr_size;
	do_div(max_pages_per_mr, (1ull << mr_page_shift));
	clt_path->max_pages_per_mr =
		min3(clt_path->max_pages_per_mr, (u32)max_pages_per_mr,
		     ib_dev->attrs.max_fast_reg_page_list_len);
	clt_path->clt->max_segments =
		min(clt_path->max_pages_per_mr, clt_path->clt->max_segments);
}