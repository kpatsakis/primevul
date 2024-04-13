static void __devexit pmcraid_remove(struct pci_dev *pdev)
{
	struct pmcraid_instance *pinstance = pci_get_drvdata(pdev);

	/* remove the management interface (/dev file) for this device */
	pmcraid_release_chrdev(pinstance);

	/* remove host template from scsi midlayer */
	scsi_remove_host(pinstance->host);

	/* block requests from mid-layer */
	scsi_block_requests(pinstance->host);

	/* initiate shutdown adapter */
	pmcraid_shutdown(pdev);

	pmcraid_disable_interrupts(pinstance, ~0);
	flush_work_sync(&pinstance->worker_q);

	pmcraid_kill_tasklets(pinstance);
	pmcraid_unregister_interrupt_handler(pinstance);
	pmcraid_release_buffers(pinstance);
	iounmap(pinstance->mapped_dma_addr);
	pci_release_regions(pdev);
	scsi_host_put(pinstance->host);
	pci_disable_device(pdev);

	return;
}
