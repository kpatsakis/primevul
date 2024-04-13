static int alloc_permits(struct rtrs_clt_sess *clt)
{
	unsigned int chunk_bits;
	int err, i;

	clt->permits_map = kcalloc(BITS_TO_LONGS(clt->queue_depth),
				   sizeof(long), GFP_KERNEL);
	if (!clt->permits_map) {
		err = -ENOMEM;
		goto out_err;
	}
	clt->permits = kcalloc(clt->queue_depth, permit_size(clt), GFP_KERNEL);
	if (!clt->permits) {
		err = -ENOMEM;
		goto err_map;
	}
	chunk_bits = ilog2(clt->queue_depth - 1) + 1;
	for (i = 0; i < clt->queue_depth; i++) {
		struct rtrs_permit *permit;

		permit = get_permit(clt, i);
		permit->mem_id = i;
		permit->mem_off = i << (MAX_IMM_PAYL_BITS - chunk_bits);
	}

	return 0;

err_map:
	kfree(clt->permits_map);
	clt->permits_map = NULL;
out_err:
	return err;
}