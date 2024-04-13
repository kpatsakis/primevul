static void pmcraid_start_bist(struct pmcraid_cmd *cmd)
{
	struct pmcraid_instance *pinstance = cmd->drv_inst;
	u32 doorbells, intrs;

	/* proceed with bist and wait for 2 seconds */
	iowrite32(DOORBELL_IOA_START_BIST,
		pinstance->int_regs.host_ioa_interrupt_reg);
	doorbells = ioread32(pinstance->int_regs.host_ioa_interrupt_reg);
	intrs = ioread32(pinstance->int_regs.ioa_host_interrupt_reg);
	pmcraid_info("doorbells after start bist: %x intrs: %x\n",
		      doorbells, intrs);

	cmd->time_left = msecs_to_jiffies(PMCRAID_BIST_TIMEOUT);
	cmd->timer.data = (unsigned long)cmd;
	cmd->timer.expires = jiffies + msecs_to_jiffies(PMCRAID_BIST_TIMEOUT);
	cmd->timer.function = (void (*)(unsigned long))pmcraid_bist_done;
	add_timer(&cmd->timer);
}
