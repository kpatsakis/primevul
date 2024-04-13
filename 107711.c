static TEE_Result param_mem_to_user_va(struct user_ta_ctx *utc,
				       struct param_mem *mem, void **user_va)
{
	struct vm_region *region;

	TAILQ_FOREACH(region, &utc->vm_info->regions, link) {
		vaddr_t va;
		size_t phys_offs;

		if (!(region->attr & TEE_MATTR_EPHEMERAL))
			continue;
		if (mem->mobj != region->mobj)
			continue;
		if (mem->offs < region->offset)
			continue;
		if (mem->offs >= (region->offset + region->size))
			continue;
		phys_offs = mobj_get_phys_offs(mem->mobj,
					       CORE_MMU_USER_PARAM_SIZE);
		va = region->va + mem->offs + phys_offs - region->offset;
		*user_va = (void *)va;
		return TEE_SUCCESS;
	}
	return TEE_ERROR_GENERIC;
}
