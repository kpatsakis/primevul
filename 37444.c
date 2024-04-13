static irqreturn_t pmcraid_isr(int irq, void *dev_id)
{
	struct pmcraid_isr_param *hrrq_vector;
	struct pmcraid_instance *pinstance;
	u32 intrs;
	unsigned long lock_flags;
	int hrrq_id = 0;

	/* In case of legacy interrupt mode where interrupts are shared across
	 * isrs, it may be possible that the current interrupt is not from IOA
	 */
	if (!dev_id) {
		printk(KERN_INFO "%s(): NULL host pointer\n", __func__);
		return IRQ_NONE;
	}
	hrrq_vector = (struct pmcraid_isr_param *)dev_id;
	pinstance = hrrq_vector->drv_inst;

	intrs = pmcraid_read_interrupts(pinstance);

	if (unlikely((intrs & PMCRAID_PCI_INTERRUPTS) == 0))
		return IRQ_NONE;

	/* Any error interrupts including unit_check, initiate IOA reset.
	 * In case of unit check indicate to reset_sequence that IOA unit
	 * checked and prepare for a dump during reset sequence
	 */
	if (intrs & PMCRAID_ERROR_INTERRUPTS) {

		if (intrs & INTRS_IOA_UNIT_CHECK)
			pinstance->ioa_unit_check = 1;

		iowrite32(intrs,
			  pinstance->int_regs.ioa_host_interrupt_clr_reg);
		pmcraid_err("ISR: error interrupts: %x initiating reset\n",
			    intrs);
		intrs = ioread32(
				pinstance->int_regs.ioa_host_interrupt_clr_reg);
		spin_lock_irqsave(pinstance->host->host_lock, lock_flags);
		pmcraid_initiate_reset(pinstance);
		spin_unlock_irqrestore(pinstance->host->host_lock, lock_flags);
	} else {
		/* If interrupt was as part of the ioa initialization,
		 * clear. Delete the timer and wakeup the
		 * reset engine to proceed with reset sequence
		 */
		if (intrs & INTRS_TRANSITION_TO_OPERATIONAL) {
			pmcraid_clr_trans_op(pinstance);
		} else {
			iowrite32(intrs,
				pinstance->int_regs.ioa_host_interrupt_clr_reg);
			ioread32(
				pinstance->int_regs.ioa_host_interrupt_clr_reg);

			tasklet_schedule(
					&(pinstance->isr_tasklet[hrrq_id]));
		}
	}

	return IRQ_HANDLED;
}
