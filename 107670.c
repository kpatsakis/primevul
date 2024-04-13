static TEE_Result add_elf_deps(struct user_ta_ctx *utc, char **deps,
			       size_t num_deps)
{
	struct user_ta_elf *libelf;
	TEE_Result res = TEE_SUCCESS;
	TEE_UUID u;
	size_t n;

	for (n = 0; n < num_deps; n++) {
		res = parse_uuid(deps[n], &u);
		if (res) {
			EMSG("Invalid dependency (not a UUID): %s", deps[n]);
			goto out;
		}
		DMSG("Library needed: %pUl", (void *)&u);
		libelf = ta_elf(&u, utc);
		if (!libelf) {
			res = TEE_ERROR_OUT_OF_MEMORY;
			goto out;
		}
	}
out:
	return res;
}
