TEE_Result tee_mmu_vbuf_to_mobj_offs(const struct user_ta_ctx *utc,
				     const void *va, size_t size,
				     struct mobj **mobj, size_t *offs)
{
	struct vm_region *r;

	TAILQ_FOREACH(r, &utc->vm_info->regions, link) {
		if (!r->mobj)
			continue;
		if (core_is_buffer_inside(va, size, r->va, r->size)) {
			size_t poffs;

			poffs = mobj_get_phys_offs(r->mobj,
						   CORE_MMU_USER_PARAM_SIZE);
			*mobj = r->mobj;
			*offs = (vaddr_t)va - r->va + r->offset - poffs;
			return TEE_SUCCESS;
		}
	}

	return TEE_ERROR_BAD_PARAMETERS;
}
