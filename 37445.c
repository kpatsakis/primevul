static irqreturn_t pmcraid_isr_msix(int irq, void *dev_id)
{
	struct pmcraid_isr_param *hrrq_vector;
	struct pmcraid_instance *pinstance;
	unsigned long lock_flags;
	u32 intrs_val;
	int hrrq_id;

	hrrq_vector = (struct pmcraid_isr_param *)dev_id;
	hrrq_id = hrrq_vector->hrrq_id;
	pinstance = hrrq_vector->drv_inst;

	if (!hrrq_id) {
		/* Read the interrupt */
		intrs_val = pmcraid_read_interrupts(pinstance);
		if (intrs_val &&
			((ioread32(pinstance->int_regs.host_ioa_interrupt_reg)
			& DOORBELL_INTR_MSIX_CLR) == 0)) {
			/* Any error interrupts including unit_check,
			 * initiate IOA reset.In case of unit check indicate
			 * to reset_sequence that IOA unit checked and prepare
			 * for a dump during reset sequence
			 */
			if (intrs_val & PMCRAID_ERROR_INTERRUPTS) {
				if (intrs_val & INTRS_IOA_UNIT_CHECK)
					pinstance->ioa_unit_check = 1;

				pmcraid_err("ISR: error interrupts: %x \
					initiating reset\n", intrs_val);
				spin_lock_irqsave(pinstance->host->host_lock,
					lock_flags);
				pmcraid_initiate_reset(pinstance);
				spin_unlock_irqrestore(
					pinstance->host->host_lock,
					lock_flags);
			}
			/* If interrupt was as part of the ioa initialization,
			 * clear it. Delete the timer and wakeup the
			 * reset engine to proceed with reset sequence
			 */
			if (intrs_val & INTRS_TRANSITION_TO_OPERATIONAL)
				pmcraid_clr_trans_op(pinstance);

			/* Clear the interrupt register by writing
			 * to host to ioa doorbell. Once done
			 * FW will clear the interrupt.
			 */
			iowrite32(DOORBELL_INTR_MSIX_CLR,
				pinstance->int_regs.host_ioa_interrupt_reg);
			ioread32(pinstance->int_regs.host_ioa_interrupt_reg);


		}
	}

	tasklet_schedule(&(pinstance->isr_tasklet[hrrq_id]));

	return IRQ_HANDLED;
}
