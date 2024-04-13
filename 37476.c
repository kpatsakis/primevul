static int pmcraid_resume(struct pci_dev *pdev)
{
	struct pmcraid_instance *pinstance = pci_get_drvdata(pdev);
	struct Scsi_Host *host = pinstance->host;
	int rc;

	pci_set_power_state(pdev, PCI_D0);
	pci_enable_wake(pdev, PCI_D0, 0);
	pci_restore_state(pdev);

	rc = pci_enable_device(pdev);

	if (rc) {
		dev_err(&pdev->dev, "resume: Enable device failed\n");
		return rc;
	}

	pci_set_master(pdev);

	if ((sizeof(dma_addr_t) == 4) ||
	     pci_set_dma_mask(pdev, DMA_BIT_MASK(64)))
		rc = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));

	if (rc == 0)
		rc = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32));

	if (rc != 0) {
		dev_err(&pdev->dev, "resume: Failed to set PCI DMA mask\n");
		goto disable_device;
	}

	pmcraid_disable_interrupts(pinstance, ~0);
	atomic_set(&pinstance->outstanding_cmds, 0);
	rc = pmcraid_register_interrupt_handler(pinstance);

	if (rc) {
		dev_err(&pdev->dev,
			"resume: couldn't register interrupt handlers\n");
		rc = -ENODEV;
		goto release_host;
	}

	pmcraid_init_tasklets(pinstance);
	pmcraid_enable_interrupts(pinstance, PMCRAID_PCI_INTERRUPTS);

	/* Start with hard reset sequence which brings up IOA to operational
	 * state as well as completes the reset sequence.
	 */
	pinstance->ioa_hard_reset = 1;

	/* Start IOA firmware initialization and bring card to Operational
	 * state.
	 */
	if (pmcraid_reset_bringup(pinstance)) {
		dev_err(&pdev->dev, "couldn't initialize IOA\n");
		rc = -ENODEV;
		goto release_tasklets;
	}

	return 0;

release_tasklets:
	pmcraid_disable_interrupts(pinstance, ~0);
	pmcraid_kill_tasklets(pinstance);
	pmcraid_unregister_interrupt_handler(pinstance);

release_host:
	scsi_host_put(host);

disable_device:
	pci_disable_device(pdev);

	return rc;
}
