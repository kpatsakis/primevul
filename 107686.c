static TEE_Result load_elf_from_store(const TEE_UUID *uuid,
				      const struct user_ta_store_ops *ta_store,
				      struct user_ta_ctx *utc)
{
	struct user_ta_store_handle *handle = NULL;
	struct elf_load_state *elf_state = NULL;
	struct ta_head *ta_head;
	struct user_ta_elf *exe;
	struct user_ta_elf *elf;
	struct user_ta_elf *prev;
	TEE_Result res;
	size_t vasize;
	void *p;
	size_t n;
	size_t num_segs = 0;
	struct load_seg *segs = NULL;

	res = ta_store->open(uuid, &handle);
	if (res)
		return res;

	elf = ta_elf(uuid, utc);
	if (!elf) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	exe = TAILQ_FIRST(&utc->elfs);
	prev = TAILQ_PREV(elf, user_ta_elf_head, link);

	res = elf_load_init(ta_store, handle, elf == exe, &utc->elfs,
			    resolve_symbol, &elf_state);
	if (res)
		goto out;
	elf->elf_state = elf_state;

	res = elf_load_head(elf_state,
			    elf == exe ? sizeof(struct ta_head) : 0,
			    &p, &vasize, &utc->is_32bit);
	if (res)
		goto out;
	ta_head = p;


	elf->mobj_code = alloc_ta_mem(vasize);
	if (!elf->mobj_code) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	if (elf == exe) {
		/* Ensure proper alignment of stack */
		size_t stack_sz = ROUNDUP(ta_head->stack_size,
					  STACK_ALIGNMENT);
		utc->mobj_stack = alloc_ta_mem(stack_sz);
		if (!utc->mobj_stack) {
			res = TEE_ERROR_OUT_OF_MEMORY;
			goto out;
		}
	}

	/*
	 * Map physical memory into TA virtual memory
	 */
	if (elf == exe) {

		res = vm_info_init(utc);
		if (res != TEE_SUCCESS)
			goto out;

		/* Add stack segment */
		utc->stack_addr = 0;
		res = vm_map(utc, &utc->stack_addr, utc->mobj_stack->size,
			     TEE_MATTR_URW | TEE_MATTR_PRW, utc->mobj_stack,
			     0);
		if (res)
			goto out;
	}

	res = get_elf_segments(elf, &segs, &num_segs);
	if (res != TEE_SUCCESS)
		goto out;

	if (prev) {
		elf->load_addr = prev->load_addr + prev->mobj_code->size;
		elf->load_addr = ROUNDUP(elf->load_addr,
					 CORE_MMU_USER_CODE_SIZE);
	}

	for (n = 0; n < num_segs; n++) {
		uint32_t prot = elf_flags_to_mattr(segs[n].flags) |
				TEE_MATTR_PRW;

		segs[n].va = elf->load_addr - segs[0].offs + segs[n].offs;
		segs[n].size = segs[n].oend - segs[n].offs;
		res = vm_map(utc, &segs[n].va, segs[n].size, prot,
			     elf->mobj_code, segs[n].offs);
		if (res)
			goto out;
		if (!n) {
			elf->load_addr = segs[0].va;
			DMSG("ELF load address %#" PRIxVA, elf->load_addr);
		}
	}

	tee_mmu_set_ctx(&utc->ctx);

	res = elf_load_body(elf_state, elf->load_addr);
	if (res)
		goto out;

	/* Find any external dependency (dynamically linked libraries) */
	res = add_deps(utc, elf_state, elf->load_addr);
out:
	if (res) {
		free(segs);
	} else {
		elf->segs = segs;
		elf->num_segs = num_segs;
	}
	ta_store->close(handle);
	/* utc is cleaned by caller on error */
	return res;
}
