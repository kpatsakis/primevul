static TEE_Result map_kinit(struct user_ta_ctx *utc __maybe_unused)
{
	TEE_Result res;
	struct mobj *mobj;
	size_t offs;
	vaddr_t va;
	size_t sz;

	thread_get_user_kcode(&mobj, &offs, &va, &sz);
	if (sz) {
		res = vm_map(utc, &va, sz, TEE_MATTR_PRX | TEE_MATTR_PERMANENT,
			     mobj, offs);
		if (res)
			return res;
	}

	thread_get_user_kdata(&mobj, &offs, &va, &sz);
	if (sz)
		return vm_map(utc, &va, sz, TEE_MATTR_PRW | TEE_MATTR_PERMANENT,
			      mobj, offs);

	return TEE_SUCCESS;
}
