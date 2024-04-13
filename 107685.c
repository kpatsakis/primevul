static TEE_Result load_elf(const TEE_UUID *uuid, struct user_ta_ctx *utc)
{
	TEE_Result res;
	const struct user_ta_store_ops *op = NULL;

	SCATTERED_ARRAY_FOREACH(op, ta_stores, struct user_ta_store_ops) {
		DMSG("Lookup user TA ELF %pUl (%s)", (void *)uuid,
		     op->description);

		res = load_elf_from_store(uuid, op, utc);
		if (res == TEE_ERROR_ITEM_NOT_FOUND)
			continue;
		if (res) {
			DMSG("res=0x%x", res);
			continue;
		}

		return res;
	}

	return TEE_ERROR_ITEM_NOT_FOUND;
}
