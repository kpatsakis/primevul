static u32 pmcraid_read_interrupts(struct pmcraid_instance *pinstance)
{
	return (pinstance->interrupt_mode) ?
		ioread32(pinstance->int_regs.ioa_host_msix_interrupt_reg) :
		ioread32(pinstance->int_regs.ioa_host_interrupt_reg);
}
