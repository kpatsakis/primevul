static TEE_Result set_exidx(struct user_ta_ctx *utc)
{
	struct user_ta_elf *exe;
	struct user_ta_elf *elf;
	struct user_ta_elf *last_elf;
	vaddr_t exidx;
	size_t exidx_sz = 0;
	TEE_Result res;
	uint8_t *p;

	if (!utc->is_32bit)
		return TEE_SUCCESS;

	exe = TAILQ_FIRST(&utc->elfs);
	if (!TAILQ_NEXT(exe, link)) {
		/* We have a single ELF: simply reference its table */
		utc->exidx_start = exe->exidx_start;
		utc->exidx_size = exe->exidx_size;
		return TEE_SUCCESS;
	}
	last_elf = TAILQ_LAST(&utc->elfs, user_ta_elf_head);

	TAILQ_FOREACH(elf, &utc->elfs, link)
		exidx_sz += elf->exidx_size;

	if (!exidx_sz) {
		/* The empty table from first segment will fit */
		utc->exidx_start = exe->exidx_start;
		utc->exidx_size = exe->exidx_size;
		return TEE_SUCCESS;
	}

	utc->mobj_exidx = alloc_ta_mem(exidx_sz);
	if (!utc->mobj_exidx)
		return TEE_ERROR_OUT_OF_MEMORY;
	exidx = ROUNDUP(last_elf->load_addr + last_elf->mobj_code->size,
			CORE_MMU_USER_CODE_SIZE);
	res = vm_map(utc, &exidx, exidx_sz, TEE_MATTR_UR | TEE_MATTR_PRW,
		     utc->mobj_exidx, 0);
	if (res)
		goto err;
	DMSG("New EXIDX table mapped at 0x%" PRIxVA " size %zu",
	     exidx, exidx_sz);

	p = (void *)exidx;
	TAILQ_FOREACH(elf, &utc->elfs, link) {
		void *e_exidx = (void *)(elf->exidx_start + elf->load_addr);
		size_t e_exidx_sz = elf->exidx_size;
		int32_t offs = (int32_t)((vaddr_t)e_exidx - (vaddr_t)p);

		memcpy(p, e_exidx, e_exidx_sz);
		res = relocate_exidx(p, e_exidx_sz, offs);
		if (res)
			goto err;
		p += e_exidx_sz;
	}

	/*
	 * Drop privileged mode permissions. Normally we should keep
	 * TEE_MATTR_PR because the code that accesses this table runs in
	 * privileged mode. However, privileged read is always enabled if
	 * unprivileged read is enabled, so it doesn't matter. For consistency
	 * with other ELF section mappings, let's clear all the privileged
	 * permission bits.
	 */
	res = vm_set_prot(utc, exidx,
			  ROUNDUP(exidx_sz, SMALL_PAGE_SIZE),
			  TEE_MATTR_UR);
	if (res)
		goto err;

	utc->exidx_start = exidx - utc->load_addr;
	utc->exidx_size = exidx_sz;

	return TEE_SUCCESS;
err:
	mobj_free(utc->mobj_exidx);
	utc->mobj_exidx = NULL;
	return res;
}
