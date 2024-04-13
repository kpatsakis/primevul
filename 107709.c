static size_t get_num_req_pgts(struct user_ta_ctx *utc, vaddr_t *begin,
			       vaddr_t *end)
{
	vaddr_t b;
	vaddr_t e;

	if (TAILQ_EMPTY(&utc->vm_info->regions)) {
		core_mmu_get_user_va_range(&b, NULL);
		e = b;
	} else {
		struct vm_region *r;

		b = TAILQ_FIRST(&utc->vm_info->regions)->va;
		r = TAILQ_LAST(&utc->vm_info->regions, vm_region_head);
		e = r->va + r->size;
		b = ROUNDDOWN(b, CORE_MMU_PGDIR_SIZE);
		e = ROUNDUP(e, CORE_MMU_PGDIR_SIZE);
	}

	if (begin)
		*begin = b;
	if (end)
		*end = e;
	return (e - b) >> CORE_MMU_PGDIR_SHIFT;
}
