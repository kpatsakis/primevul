TEE_Result tee_ta_init_user_ta_session(const TEE_UUID *uuid,
				       struct tee_ta_session *s)
{
	TEE_Result res;
	struct user_ta_ctx *utc = NULL;
	struct ta_head *ta_head;
	struct user_ta_elf *exe;
	struct user_ta_elf *elf;

	/* Register context */
	utc = calloc(1, sizeof(struct user_ta_ctx));
	if (!utc)
		return TEE_ERROR_OUT_OF_MEMORY;

	TAILQ_INIT(&utc->open_sessions);
	TAILQ_INIT(&utc->cryp_states);
	TAILQ_INIT(&utc->objects);
	TAILQ_INIT(&utc->storage_enums);
	TAILQ_INIT(&utc->elfs);

	/*
	 * Set context TA operation structure. It is required by generic
	 * implementation to identify userland TA versus pseudo TA contexts.
	 */
	set_ta_ctx_ops(&utc->ctx);

	/*
	 * Create entry for the main executable
	 */
	exe = ta_elf(uuid, utc);
	if (!exe) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto err;
	}

	/*
	 * Load binaries and map them into the TA virtual memory. load_elf()
	 * may add external libraries to the list, so the loop will end when
	 * all the dependencies are satisfied or an error occurs.
	 */
	TAILQ_FOREACH(elf, &utc->elfs, link) {
		res = load_elf(&elf->uuid, utc);
		if (res)
			goto err;
	}

	/*
	 * Perform relocations and apply final memory attributes
	 */
	TAILQ_FOREACH(elf, &utc->elfs, link) {
		DMSG("Processing relocations in %pUl", (void *)&elf->uuid);
		res = elf_process_rel(elf->elf_state, elf->load_addr);
		if (res)
			goto err;
		res = set_seg_prot(utc, elf);
		if (res)
			goto err;
	}

	utc->load_addr = exe->load_addr;
	res = set_exidx(utc);
	if (res)
		goto err;

	ta_head = (struct ta_head *)(vaddr_t)utc->load_addr;

	if (memcmp(&ta_head->uuid, uuid, sizeof(TEE_UUID)) != 0) {
		res = TEE_ERROR_SECURITY;
		goto err;
	}

	if (ta_head->flags & ~TA_FLAGS_MASK) {
		EMSG("Invalid TA flag(s) 0x%" PRIx32,
			ta_head->flags & ~TA_FLAGS_MASK);
		res = TEE_ERROR_BAD_FORMAT;
		goto err;
	}

	utc->ctx.flags = ta_head->flags;
	utc->ctx.uuid = ta_head->uuid;
	utc->entry_func = ta_head->entry.ptr64;
	utc->ctx.ref_count = 1;
	condvar_init(&utc->ctx.busy_cv);
	TAILQ_INSERT_TAIL(&tee_ctxes, &utc->ctx, link);
	s->ctx = &utc->ctx;

	free_elf_states(utc);
	tee_mmu_set_ctx(NULL);
	return TEE_SUCCESS;

err:
	free_elf_states(utc);
	tee_mmu_set_ctx(NULL);
	free_utc(utc);
	return res;
}
