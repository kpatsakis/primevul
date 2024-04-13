static TEE_Result resolve_symbol(struct user_ta_elf_head *elfs,
				 const char *name, uintptr_t *val)
{
	struct user_ta_elf *elf;
	TEE_Result res;

	/*
	 * The loop naturally implements a breadth first search due to the
	 * order in which the libraries were added.
	 */
	TAILQ_FOREACH(elf, elfs, link) {
		res = elf_resolve_symbol(elf->elf_state, name, val);
		if (res == TEE_ERROR_ITEM_NOT_FOUND)
			continue;
		if (res)
			return res;
		*val += elf->load_addr;
		FMSG("%pUl/0x%" PRIxPTR " %s", (void *)&elf->uuid, *val, name);
		return TEE_SUCCESS;
	}

	return TEE_ERROR_ITEM_NOT_FOUND;
}
