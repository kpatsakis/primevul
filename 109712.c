static unsigned long vdso_addr(unsigned long start, unsigned len)
{
#ifdef CONFIG_X86_32
	return 0;
#else
	unsigned long addr, end;
	unsigned offset;
	end = (start + PMD_SIZE - 1) & PMD_MASK;
	if (end >= TASK_SIZE_MAX)
		end = TASK_SIZE_MAX;
	end -= len;
	/* This loses some more bits than a modulo, but is cheaper */
	offset = get_random_int() & (PTRS_PER_PTE - 1);
	addr = start + (offset << PAGE_SHIFT);
	if (addr >= end)
		addr = end;

	/*
	 * page-align it here so that get_unmapped_area doesn't
	 * align it wrongfully again to the next page. addr can come in 4K
	 * unaligned here as a result of stack start randomization.
	 */
	addr = PAGE_ALIGN(addr);
	addr = align_vdso_addr(addr);

	return addr;
#endif
}