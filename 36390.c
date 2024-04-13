static inline void perf_get_data_addr(struct pt_regs *regs, u64 *addrp)
{
	unsigned long mmcra = regs->dsisr;
	unsigned long sdsync = (ppmu->flags & PPMU_ALT_SIPR) ?
		POWER6_MMCRA_SDSYNC : MMCRA_SDSYNC;

	if (!(mmcra & MMCRA_SAMPLE_ENABLE) || (mmcra & sdsync))
		*addrp = mfspr(SPRN_SDAR);
}
