void elf32_core_copy_regs(elf_gregset_t grp, struct pt_regs *regs)
{
	int i;

	for (i = 0; i < EF_R0; i++)
		grp[i] = 0;
	grp[EF_R0] = 0;
	for (i = 1; i <= 31; i++)
		grp[EF_R0 + i] = (elf_greg_t) regs->regs[i];
	grp[EF_R26] = 0;
	grp[EF_R27] = 0;
	grp[EF_LO] = (elf_greg_t) regs->lo;
	grp[EF_HI] = (elf_greg_t) regs->hi;
	grp[EF_CP0_EPC] = (elf_greg_t) regs->cp0_epc;
	grp[EF_CP0_BADVADDR] = (elf_greg_t) regs->cp0_badvaddr;
	grp[EF_CP0_STATUS] = (elf_greg_t) regs->cp0_status;
	grp[EF_CP0_CAUSE] = (elf_greg_t) regs->cp0_cause;
#ifdef EF_UNUSED0
	grp[EF_UNUSED0] = 0;
#endif
}
