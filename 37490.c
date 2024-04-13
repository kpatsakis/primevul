static void pmcraid_soft_reset(struct pmcraid_cmd *cmd)
{
	struct pmcraid_instance *pinstance = cmd->drv_inst;
	u32 int_reg;
	u32 doorbell;

	/* There will be an interrupt when Transition to Operational bit is
	 * set so tasklet would execute next reset task. The timeout handler
	 * would re-initiate a reset
	 */
	cmd->cmd_done = pmcraid_ioa_reset;
	cmd->timer.data = (unsigned long)cmd;
	cmd->timer.expires = jiffies +
			     msecs_to_jiffies(PMCRAID_TRANSOP_TIMEOUT);
	cmd->timer.function = (void (*)(unsigned long))pmcraid_timeout_handler;

	if (!timer_pending(&cmd->timer))
		add_timer(&cmd->timer);

	/* Enable destructive diagnostics on IOA if it is not yet in
	 * operational state
	 */
	doorbell = DOORBELL_RUNTIME_RESET |
		   DOORBELL_ENABLE_DESTRUCTIVE_DIAGS;

	/* Since we do RESET_ALERT and Start BIST we have to again write
	 * MSIX Doorbell to indicate the interrupt mode
	 */
	if (pinstance->interrupt_mode) {
		iowrite32(DOORBELL_INTR_MODE_MSIX,
			  pinstance->int_regs.host_ioa_interrupt_reg);
		ioread32(pinstance->int_regs.host_ioa_interrupt_reg);
	}

	iowrite32(doorbell, pinstance->int_regs.host_ioa_interrupt_reg);
	ioread32(pinstance->int_regs.host_ioa_interrupt_reg),
	int_reg = ioread32(pinstance->int_regs.ioa_host_interrupt_reg);

	pmcraid_info("Waiting for IOA to become operational %x:%x\n",
		     ioread32(pinstance->int_regs.host_ioa_interrupt_reg),
		     int_reg);
}
