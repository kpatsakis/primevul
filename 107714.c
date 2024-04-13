struct tee_ta_ctx *tee_mmu_get_ctx(void)
{
	return thread_get_tsd()->ctx;
}
