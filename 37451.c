static int __devinit pmcraid_probe(
	struct pci_dev *pdev,
	const struct pci_device_id *dev_id
)
{
	struct pmcraid_instance *pinstance;
	struct Scsi_Host *host;
	void __iomem *mapped_pci_addr;
	int rc = PCIBIOS_SUCCESSFUL;

	if (atomic_read(&pmcraid_adapter_count) >= PMCRAID_MAX_ADAPTERS) {
		pmcraid_err
			("maximum number(%d) of supported adapters reached\n",
			 atomic_read(&pmcraid_adapter_count));
		return -ENOMEM;
	}

	atomic_inc(&pmcraid_adapter_count);
	rc = pci_enable_device(pdev);

	if (rc) {
		dev_err(&pdev->dev, "Cannot enable adapter\n");
		atomic_dec(&pmcraid_adapter_count);
		return rc;
	}

	dev_info(&pdev->dev,
		"Found new IOA(%x:%x), Total IOA count: %d\n",
		 pdev->vendor, pdev->device,
		 atomic_read(&pmcraid_adapter_count));

	rc = pci_request_regions(pdev, PMCRAID_DRIVER_NAME);

	if (rc < 0) {
		dev_err(&pdev->dev,
			"Couldn't register memory range of registers\n");
		goto out_disable_device;
	}

	mapped_pci_addr = pci_iomap(pdev, 0, 0);

	if (!mapped_pci_addr) {
		dev_err(&pdev->dev, "Couldn't map PCI registers memory\n");
		rc = -ENOMEM;
		goto out_release_regions;
	}

	pci_set_master(pdev);

	/* Firmware requires the system bus address of IOARCB to be within
	 * 32-bit addressable range though it has 64-bit IOARRIN register.
	 * However, firmware supports 64-bit streaming DMA buffers, whereas
	 * coherent buffers are to be 32-bit. Since pci_alloc_consistent always
	 * returns memory within 4GB (if not, change this logic), coherent
	 * buffers are within firmware acceptable address ranges.
	 */
	if ((sizeof(dma_addr_t) == 4) ||
	    pci_set_dma_mask(pdev, DMA_BIT_MASK(64)))
		rc = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));

	/* firmware expects 32-bit DMA addresses for IOARRIN register; set 32
	 * bit mask for pci_alloc_consistent to return addresses within 4GB
	 */
	if (rc == 0)
		rc = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32));

	if (rc != 0) {
		dev_err(&pdev->dev, "Failed to set PCI DMA mask\n");
		goto cleanup_nomem;
	}

	host = scsi_host_alloc(&pmcraid_host_template,
				sizeof(struct pmcraid_instance));

	if (!host) {
		dev_err(&pdev->dev, "scsi_host_alloc failed!\n");
		rc = -ENOMEM;
		goto cleanup_nomem;
	}

	host->max_id = PMCRAID_MAX_NUM_TARGETS_PER_BUS;
	host->max_lun = PMCRAID_MAX_NUM_LUNS_PER_TARGET;
	host->unique_id = host->host_no;
	host->max_channel = PMCRAID_MAX_BUS_TO_SCAN;
	host->max_cmd_len = PMCRAID_MAX_CDB_LEN;

	/* zero out entire instance structure */
	pinstance = (struct pmcraid_instance *)host->hostdata;
	memset(pinstance, 0, sizeof(*pinstance));

	pinstance->chip_cfg =
		(struct pmcraid_chip_details *)(dev_id->driver_data);

	rc = pmcraid_init_instance(pdev, host, mapped_pci_addr);

	if (rc < 0) {
		dev_err(&pdev->dev, "failed to initialize adapter instance\n");
		goto out_scsi_host_put;
	}

	pci_set_drvdata(pdev, pinstance);

	/* Save PCI config-space for use following the reset */
	rc = pci_save_state(pinstance->pdev);

	if (rc != 0) {
		dev_err(&pdev->dev, "Failed to save PCI config space\n");
		goto out_scsi_host_put;
	}

	pmcraid_disable_interrupts(pinstance, ~0);

	rc = pmcraid_register_interrupt_handler(pinstance);

	if (rc) {
		dev_err(&pdev->dev, "couldn't register interrupt handler\n");
		goto out_scsi_host_put;
	}

	pmcraid_init_tasklets(pinstance);

	/* allocate verious buffers used by LLD.*/
	rc = pmcraid_init_buffers(pinstance);

	if (rc) {
		pmcraid_err("couldn't allocate memory blocks\n");
		goto out_unregister_isr;
	}

	/* check the reset type required */
	pmcraid_reset_type(pinstance);

	pmcraid_enable_interrupts(pinstance, PMCRAID_PCI_INTERRUPTS);

	/* Start IOA firmware initialization and bring card to Operational
	 * state.
	 */
	pmcraid_info("starting IOA initialization sequence\n");
	if (pmcraid_reset_bringup(pinstance)) {
		dev_err(&pdev->dev, "couldn't initialize IOA\n");
		rc = 1;
		goto out_release_bufs;
	}

	/* Add adapter instance into mid-layer list */
	rc = scsi_add_host(pinstance->host, &pdev->dev);
	if (rc != 0) {
		pmcraid_err("couldn't add host into mid-layer: %d\n", rc);
		goto out_release_bufs;
	}

	scsi_scan_host(pinstance->host);

	rc = pmcraid_setup_chrdev(pinstance);

	if (rc != 0) {
		pmcraid_err("couldn't create mgmt interface, error: %x\n",
			     rc);
		goto out_remove_host;
	}

	/* Schedule worker thread to handle CCN and take care of adding and
	 * removing devices to OS
	 */
	atomic_set(&pinstance->expose_resources, 1);
	schedule_work(&pinstance->worker_q);
	return rc;

out_remove_host:
	scsi_remove_host(host);

out_release_bufs:
	pmcraid_release_buffers(pinstance);

out_unregister_isr:
	pmcraid_kill_tasklets(pinstance);
	pmcraid_unregister_interrupt_handler(pinstance);

out_scsi_host_put:
	scsi_host_put(host);

cleanup_nomem:
	iounmap(mapped_pci_addr);

out_release_regions:
	pci_release_regions(pdev);

out_disable_device:
	atomic_dec(&pmcraid_adapter_count);
	pci_set_drvdata(pdev, NULL);
	pci_disable_device(pdev);
	return -ENODEV;
}
