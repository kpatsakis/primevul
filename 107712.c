static vaddr_t select_va_in_range(vaddr_t prev_end, uint32_t prev_attr,
				  vaddr_t next_begin, uint32_t next_attr,
				  const struct vm_region *reg)
{
	size_t granul;
	const uint32_t a = TEE_MATTR_EPHEMERAL | TEE_MATTR_PERMANENT;
	size_t pad;
	vaddr_t begin_va;
	vaddr_t end_va;

	/*
	 * Insert an unmapped entry to separate regions with differing
	 * TEE_MATTR_EPHEMERAL TEE_MATTR_PERMANENT bits as they never are
	 * to be contiguous with another region.
	 */
	if (prev_attr && (prev_attr & a) != (reg->attr & a))
		pad = SMALL_PAGE_SIZE;
	else
		pad = 0;

	granul = SMALL_PAGE_SIZE;
#ifndef CFG_WITH_LPAE
	if ((prev_attr & TEE_MATTR_SECURE) != (reg->attr & TEE_MATTR_SECURE))
		granul = CORE_MMU_PGDIR_SIZE;
#endif
	begin_va = ROUNDUP(prev_end + pad, granul);
	if (reg->va) {
		if (reg->va < begin_va)
			return 0;
		begin_va = reg->va;
	}

	if (next_attr && (next_attr & a) != (reg->attr & a))
		pad = SMALL_PAGE_SIZE;
	else
		pad = 0;

	granul = SMALL_PAGE_SIZE;
#ifndef CFG_WITH_LPAE
	if ((next_attr & TEE_MATTR_SECURE) != (reg->attr & TEE_MATTR_SECURE))
		granul = CORE_MMU_PGDIR_SIZE;
#endif
	end_va = ROUNDUP(begin_va + reg->size + pad, granul);

	if (end_va <= next_begin) {
		assert(!reg->va || reg->va == begin_va);
		return begin_va;
	}

	return 0;
}
