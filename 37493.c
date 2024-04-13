static int pmcraid_suspend(struct pci_dev *pdev, pm_message_t state)
{
	struct pmcraid_instance *pinstance = pci_get_drvdata(pdev);

	pmcraid_shutdown(pdev);
	pmcraid_disable_interrupts(pinstance, ~0);
	pmcraid_kill_tasklets(pinstance);
	pci_set_drvdata(pinstance->pdev, pinstance);
	pmcraid_unregister_interrupt_handler(pinstance);
	pci_save_state(pdev);
	pci_disable_device(pdev);
	pci_set_power_state(pdev, pci_choose_state(pdev, state));

	return 0;
}
