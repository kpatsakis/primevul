static TEE_Result umap_add_region(struct vm_info *vmi, struct vm_region *reg)
{
	struct vm_region *r;
	struct vm_region *prev_r;
	vaddr_t va_range_base;
	size_t va_range_size;
	vaddr_t va;

	core_mmu_get_user_va_range(&va_range_base, &va_range_size);

	/* Check alignment, it has to be at least SMALL_PAGE based */
	if ((reg->va | reg->size) & SMALL_PAGE_MASK)
		return TEE_ERROR_ACCESS_CONFLICT;

	/* Check that the mobj is defined for the entire range */
	if ((reg->offset + reg->size) >
	     ROUNDUP(reg->mobj->size, SMALL_PAGE_SIZE))
		return TEE_ERROR_BAD_PARAMETERS;

	prev_r = NULL;
	TAILQ_FOREACH(r, &vmi->regions, link) {
		if (TAILQ_FIRST(&vmi->regions) == r) {
			va = select_va_in_range(va_range_base, 0,
						r->va, r->attr, reg);
			if (va) {
				reg->va = va;
				TAILQ_INSERT_HEAD(&vmi->regions, reg, link);
				return TEE_SUCCESS;
			}
		} else {
			va = select_va_in_range(prev_r->va + prev_r->size,
						prev_r->attr, r->va, r->attr,
						reg);
			if (va) {
				reg->va = va;
				TAILQ_INSERT_BEFORE(r, reg, link);
				return TEE_SUCCESS;
			}
		}
		prev_r = r;
	}

	r = TAILQ_LAST(&vmi->regions, vm_region_head);
	if (r) {
		va = select_va_in_range(r->va + r->size, r->attr,
					va_range_base + va_range_size, 0, reg);
		if (va) {
			reg->va = va;
			TAILQ_INSERT_TAIL(&vmi->regions, reg, link);
			return TEE_SUCCESS;
		}
	} else {
		va = select_va_in_range(va_range_base, 0,
					va_range_base + va_range_size, 0, reg);
		if (va) {
			reg->va = va;
			TAILQ_INSERT_HEAD(&vmi->regions, reg, link);
			return TEE_SUCCESS;
		}
	}

	return TEE_ERROR_ACCESS_CONFLICT;
}
