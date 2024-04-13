TEE_Result tee_mmu_add_rwmem(struct user_ta_ctx *utc, struct mobj *mobj,
			     vaddr_t *va)
{
	TEE_Result res;
	struct vm_region *reg = calloc(1, sizeof(*reg));

	if (!reg)
		return TEE_ERROR_OUT_OF_MEMORY;

	reg->mobj = mobj;
	reg->offset = 0;
	reg->va = 0;
	reg->size = ROUNDUP(mobj->size, SMALL_PAGE_SIZE);
	if (mobj_is_secure(mobj))
		reg->attr = TEE_MATTR_SECURE;
	else
		reg->attr = 0;

	res = umap_add_region(utc->vm_info, reg);
	if (res) {
		free(reg);
		return res;
	}

	res = alloc_pgt(utc);
	if (res)
		umap_remove_region(utc->vm_info, reg);
	else
		*va = reg->va;

	return res;
}
