TEE_Result vm_set_prot(struct user_ta_ctx *utc, vaddr_t va, size_t len,
		       uint32_t prot)
{
	struct vm_region *r;

	/*
	 * To keep thing simple: specified va and len has to match exactly
	 * with an already registered region.
	 */
	TAILQ_FOREACH(r, &utc->vm_info->regions, link) {
		if (core_is_buffer_intersect(r->va, r->size, va, len)) {
			if (r->va != va || r->size != len)
				return TEE_ERROR_BAD_PARAMETERS;
			if (mobj_is_paged(r->mobj)) {
				if (!tee_pager_set_uta_area_attr(utc, va, len,
								 prot))
					return TEE_ERROR_GENERIC;
			} else if ((prot & TEE_MATTR_UX) &&
				   (r->attr & (TEE_MATTR_UW | TEE_MATTR_PW))) {
				cache_op_inner(DCACHE_AREA_CLEAN,
					       (void *)va, len);
				cache_op_inner(ICACHE_AREA_INVALIDATE,
					       (void *)va, len);
			}
			r->attr &= ~TEE_MATTR_PROT_MASK;
			r->attr |= prot & TEE_MATTR_PROT_MASK;
			return TEE_SUCCESS;
		}
	}

	return TEE_ERROR_ITEM_NOT_FOUND;
}
