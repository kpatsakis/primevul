static void free_pgt(struct user_ta_ctx *utc, vaddr_t base, size_t size)
{
	struct thread_specific_data *tsd = thread_get_tsd();
	struct pgt_cache *pgt_cache = NULL;

	if (&utc->ctx == tsd->ctx)
		pgt_cache = &tsd->pgt_cache;

	pgt_flush_ctx_range(pgt_cache, &utc->ctx, base, base + size);
}
