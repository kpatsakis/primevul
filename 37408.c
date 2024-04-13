static void pmcraid_disable_interrupts(
	struct pmcraid_instance *pinstance,
	u32 intrs
)
{
	u32 gmask = ioread32(pinstance->int_regs.global_interrupt_mask_reg);
	u32 nmask = gmask | GLOBAL_INTERRUPT_MASK;

	iowrite32(intrs, pinstance->int_regs.ioa_host_interrupt_clr_reg);
	iowrite32(nmask, pinstance->int_regs.global_interrupt_mask_reg);
	ioread32(pinstance->int_regs.global_interrupt_mask_reg);

	if (!pinstance->interrupt_mode) {
		iowrite32(intrs,
			pinstance->int_regs.ioa_host_interrupt_mask_reg);
		ioread32(pinstance->int_regs.ioa_host_interrupt_mask_reg);
	}
}
