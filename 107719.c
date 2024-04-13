uint32_t tee_mmu_user_get_cache_attr(struct user_ta_ctx *utc, void *va)
{
	uint32_t attr;

	if (tee_mmu_user_va2pa_attr(utc, va, NULL, &attr) != TEE_SUCCESS)
		panic("cannot get attr");

	return (attr >> TEE_MATTR_CACHE_SHIFT) & TEE_MATTR_CACHE_MASK;
}
