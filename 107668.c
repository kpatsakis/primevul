static TEE_Result add_deps(struct user_ta_ctx *utc,
			   struct elf_load_state *state, vaddr_t load_addr)
{
	char **deps = NULL;
	size_t num_deps = 0;
	TEE_Result res;

	res = elf_get_needed(state, load_addr, &deps, &num_deps);
	if (res)
		return res;

	res = add_elf_deps(utc, deps, num_deps);
	free(deps);

	return res;
}
