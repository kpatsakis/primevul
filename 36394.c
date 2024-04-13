static inline int perf_intr_is_nmi(struct pt_regs *regs)
{
	return !regs->softe;
}
