static void pmcraid_clr_trans_op(
	struct pmcraid_instance *pinstance
)
{
	unsigned long lock_flags;

	if (!pinstance->interrupt_mode) {
		iowrite32(INTRS_TRANSITION_TO_OPERATIONAL,
			pinstance->int_regs.ioa_host_interrupt_mask_reg);
		ioread32(pinstance->int_regs.ioa_host_interrupt_mask_reg);
		iowrite32(INTRS_TRANSITION_TO_OPERATIONAL,
			pinstance->int_regs.ioa_host_interrupt_clr_reg);
		ioread32(pinstance->int_regs.ioa_host_interrupt_clr_reg);
	}

	if (pinstance->reset_cmd != NULL) {
		del_timer(&pinstance->reset_cmd->timer);
		spin_lock_irqsave(
			pinstance->host->host_lock, lock_flags);
		pinstance->reset_cmd->cmd_done(pinstance->reset_cmd);
		spin_unlock_irqrestore(
			pinstance->host->host_lock, lock_flags);
	}
}
