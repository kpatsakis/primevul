TEE_Result tee_mmu_user_pa2va_helper(const struct user_ta_ctx *utc,
				      paddr_t pa, void **va)
{
	TEE_Result res;
	paddr_t p;
	struct vm_region *region;

	TAILQ_FOREACH(region, &utc->vm_info->regions, link) {
		size_t granule;
		size_t size;
		size_t ofs;

		/* pa2va is expected only for memory tracked through mobj */
		if (!region->mobj)
			continue;

		/* Physically granulated memory object must be scanned */
		granule = region->mobj->phys_granule;
		assert(!granule || IS_POWER_OF_TWO(granule));

		for (ofs = region->offset; ofs < region->size; ofs += size) {

			if (granule) {
				/* From current offset to buffer/granule end */
				size = granule - (ofs & (granule - 1));

				if (size > (region->size - ofs))
					size = region->size - ofs;
			} else
				size = region->size;

			res = mobj_get_pa(region->mobj, ofs, granule, &p);
			if (res != TEE_SUCCESS)
				return res;

			if (core_is_buffer_inside(pa, 1, p, size)) {
				/* Remove region offset (mobj phys offset) */
				ofs -= region->offset;
				/* Get offset-in-granule */
				p = pa - p;

				*va = (void *)(region->va + ofs + (vaddr_t)p);
				return TEE_SUCCESS;
			}
		}
	}

	return TEE_ERROR_ACCESS_DENIED;
}
