static int __devinit pmcraid_init_instance(
	struct pci_dev *pdev,
	struct Scsi_Host *host,
	void __iomem *mapped_pci_addr
)
{
	struct pmcraid_instance *pinstance =
		(struct pmcraid_instance *)host->hostdata;

	pinstance->host = host;
	pinstance->pdev = pdev;

	/* Initialize register addresses */
	pinstance->mapped_dma_addr = mapped_pci_addr;

	/* Initialize chip-specific details */
	{
		struct pmcraid_chip_details *chip_cfg = pinstance->chip_cfg;
		struct pmcraid_interrupts *pint_regs = &pinstance->int_regs;

		pinstance->ioarrin = mapped_pci_addr + chip_cfg->ioarrin;

		pint_regs->ioa_host_interrupt_reg =
			mapped_pci_addr + chip_cfg->ioa_host_intr;
		pint_regs->ioa_host_interrupt_clr_reg =
			mapped_pci_addr + chip_cfg->ioa_host_intr_clr;
		pint_regs->ioa_host_msix_interrupt_reg =
			mapped_pci_addr + chip_cfg->ioa_host_msix_intr;
		pint_regs->host_ioa_interrupt_reg =
			mapped_pci_addr + chip_cfg->host_ioa_intr;
		pint_regs->host_ioa_interrupt_clr_reg =
			mapped_pci_addr + chip_cfg->host_ioa_intr_clr;

		/* Current version of firmware exposes interrupt mask set
		 * and mask clr registers through memory mapped bar0.
		 */
		pinstance->mailbox = mapped_pci_addr + chip_cfg->mailbox;
		pinstance->ioa_status = mapped_pci_addr + chip_cfg->ioastatus;
		pint_regs->ioa_host_interrupt_mask_reg =
			mapped_pci_addr + chip_cfg->ioa_host_mask;
		pint_regs->ioa_host_interrupt_mask_clr_reg =
			mapped_pci_addr + chip_cfg->ioa_host_mask_clr;
		pint_regs->global_interrupt_mask_reg =
			mapped_pci_addr + chip_cfg->global_intr_mask;
	};

	pinstance->ioa_reset_attempts = 0;
	init_waitqueue_head(&pinstance->reset_wait_q);

	atomic_set(&pinstance->outstanding_cmds, 0);
	atomic_set(&pinstance->last_message_id, 0);
	atomic_set(&pinstance->expose_resources, 0);

	INIT_LIST_HEAD(&pinstance->free_res_q);
	INIT_LIST_HEAD(&pinstance->used_res_q);
	INIT_LIST_HEAD(&pinstance->free_cmd_pool);
	INIT_LIST_HEAD(&pinstance->pending_cmd_pool);

	spin_lock_init(&pinstance->free_pool_lock);
	spin_lock_init(&pinstance->pending_pool_lock);
	spin_lock_init(&pinstance->resource_lock);
	mutex_init(&pinstance->aen_queue_lock);

	/* Work-queue (Shared) for deferred processing error handling */
	INIT_WORK(&pinstance->worker_q, pmcraid_worker_function);

	/* Initialize the default log_level */
	pinstance->current_log_level = pmcraid_log_level;

	/* Setup variables required for reset engine */
	pinstance->ioa_state = IOA_STATE_UNKNOWN;
	pinstance->reset_cmd = NULL;
	return 0;
}
