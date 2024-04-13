static TEE_Result tee_mmu_user_va2pa_attr(const struct user_ta_ctx *utc,
			void *ua, paddr_t *pa, uint32_t *attr)
{
	struct vm_region *region;

	TAILQ_FOREACH(region, &utc->vm_info->regions, link) {
		if (!core_is_buffer_inside(ua, 1, region->va, region->size))
			continue;

		if (pa) {
			TEE_Result res;
			paddr_t p;
			size_t offset;
			size_t granule;

			/*
			 * mobj and input user address may each include
			 * a specific offset-in-granule position.
			 * Drop both to get target physical page base
			 * address then apply only user address
			 * offset-in-granule.
			 * Mapping lowest granule is the small page.
			 */
			granule = MAX(region->mobj->phys_granule,
				      (size_t)SMALL_PAGE_SIZE);
			assert(!granule || IS_POWER_OF_TWO(granule));

			offset = region->offset +
				 ROUNDDOWN((vaddr_t)ua - region->va, granule);

			res = mobj_get_pa(region->mobj, offset, granule, &p);
			if (res != TEE_SUCCESS)
				return res;

			*pa = p | ((vaddr_t)ua & (granule - 1));
		}
		if (attr)
			*attr = region->attr;

		return TEE_SUCCESS;
	}

	return TEE_ERROR_ACCESS_DENIED;
}
