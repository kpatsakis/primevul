static inline void __rtrs_put_permit(struct rtrs_clt_sess *clt,
				      struct rtrs_permit *permit)
{
	clear_bit_unlock(permit->mem_id, clt->permits_map);
}