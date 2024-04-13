TEE_Result vm_info_init(struct user_ta_ctx *utc)
{
	TEE_Result res;
	uint32_t asid = asid_alloc();

	if (!asid) {
		DMSG("Failed to allocate ASID");
		return TEE_ERROR_GENERIC;
	}

	utc->vm_info = calloc(1, sizeof(*utc->vm_info));
	if (!utc->vm_info) {
		asid_free(asid);
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	TAILQ_INIT(&utc->vm_info->regions);
	utc->vm_info->asid = asid;

	res = map_kinit(utc);
	if (res)
		vm_info_final(utc);
	return res;
}
