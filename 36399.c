static inline void perf_read_regs(struct pt_regs *regs)
{
	regs->dsisr = mfspr(SPRN_MMCRA);
}
