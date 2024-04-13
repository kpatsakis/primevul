static inline u32 perf_get_misc_flags(struct pt_regs *regs)
{
	unsigned long mmcra = regs->dsisr;
	unsigned long sihv = MMCRA_SIHV;
	unsigned long sipr = MMCRA_SIPR;

	if (TRAP(regs) != 0xf00)
		return 0;	/* not a PMU interrupt */

	if (ppmu->flags & PPMU_ALT_SIPR) {
		sihv = POWER6_MMCRA_SIHV;
		sipr = POWER6_MMCRA_SIPR;
	}

	/* PR has priority over HV, so order below is important */
	if (mmcra & sipr)
		return PERF_RECORD_MISC_USER;
	if ((mmcra & sihv) && (freeze_events_kernel != MMCR0_FCHV))
		return PERF_RECORD_MISC_HYPERVISOR;
	return PERF_RECORD_MISC_KERNEL;
}
