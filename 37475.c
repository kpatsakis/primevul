static int pmcraid_reset_enable_ioa(struct pmcraid_instance *pinstance)
{
	u32 intrs;

	pmcraid_reinit_buffers(pinstance);
	intrs = pmcraid_read_interrupts(pinstance);

	pmcraid_enable_interrupts(pinstance, PMCRAID_PCI_INTERRUPTS);

	if (intrs & INTRS_TRANSITION_TO_OPERATIONAL) {
		if (!pinstance->interrupt_mode) {
			iowrite32(INTRS_TRANSITION_TO_OPERATIONAL,
				pinstance->int_regs.
				ioa_host_interrupt_mask_reg);
			iowrite32(INTRS_TRANSITION_TO_OPERATIONAL,
				pinstance->int_regs.ioa_host_interrupt_clr_reg);
		}
		return 1;
	} else {
		return 0;
	}
}
