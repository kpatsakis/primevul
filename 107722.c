TEE_Result tee_mmu_user_va2pa_helper(const struct user_ta_ctx *utc, void *ua,
				     paddr_t *pa)
{
	return tee_mmu_user_va2pa_attr(utc, ua, pa, NULL);
}
