TEE_Result vm_map(struct user_ta_ctx *utc, vaddr_t *va, size_t len,
		  uint32_t prot, struct mobj *mobj, size_t offs)
{
	TEE_Result res;
	struct vm_region *reg = calloc(1, sizeof(*reg));
	uint32_t attr = 0;
	const uint32_t prot_mask = TEE_MATTR_PROT_MASK | TEE_MATTR_PERMANENT |
				   TEE_MATTR_EPHEMERAL;

	if (!reg)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (prot & ~prot_mask) {
		res = TEE_ERROR_BAD_PARAMETERS;
		goto err_free_reg;
	}

	if (!mobj_is_paged(mobj)) {
		uint32_t cattr;

		res = mobj_get_cattr(mobj, &cattr);
		if (res)
			goto err_free_reg;
		attr |= cattr << TEE_MATTR_CACHE_SHIFT;
	}
	attr |= TEE_MATTR_VALID_BLOCK;
	if (mobj_is_secure(mobj))
		attr |= TEE_MATTR_SECURE;

	reg->mobj = mobj;
	reg->offset = offs;
	reg->va = *va;
	reg->size = ROUNDUP(len, SMALL_PAGE_SIZE);
	reg->attr = attr | prot;

	res = umap_add_region(utc->vm_info, reg);
	if (res)
		goto err_free_reg;

	res = alloc_pgt(utc);
	if (res)
		goto err_rem_reg;

	if (!(reg->attr & (TEE_MATTR_EPHEMERAL | TEE_MATTR_PERMANENT)) &&
	    mobj_is_paged(mobj)) {
		if (!tee_pager_add_uta_area(utc, reg->va, reg->size)) {
			res = TEE_ERROR_GENERIC;
			goto err_rem_reg;
		}
	}

	/*
	 * If the context currently is active set it again to update
	 * the mapping.
	 */
	if (thread_get_tsd()->ctx == &utc->ctx)
		tee_mmu_set_ctx(&utc->ctx);

	*va = reg->va;

	return TEE_SUCCESS;

err_rem_reg:
	TAILQ_REMOVE(&utc->vm_info->regions, reg, link);
err_free_reg:
	free(reg);
	return res;
}
