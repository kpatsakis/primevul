static TEE_Result alloc_pgt(struct user_ta_ctx *utc)
{
	struct thread_specific_data *tsd __maybe_unused;
	vaddr_t b;
	vaddr_t e;
	size_t ntbl;

	ntbl = get_num_req_pgts(utc, &b, &e);
	if (!pgt_check_avail(ntbl)) {
		EMSG("%zu page tables not available", ntbl);
		return TEE_ERROR_OUT_OF_MEMORY;
	}

#ifdef CFG_PAGED_USER_TA
	tsd = thread_get_tsd();
	if (&utc->ctx == tsd->ctx) {
		/*
		 * The supplied utc is the current active utc, allocate the
		 * page tables too as the pager needs to use them soon.
		 */
		pgt_alloc(&tsd->pgt_cache, &utc->ctx, b, e - 1);
	}
#endif

	return TEE_SUCCESS;
}
