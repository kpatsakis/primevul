void pmcraid_unregister_interrupt_handler(struct pmcraid_instance *pinstance)
{
	int i;

	for (i = 0; i < pinstance->num_hrrq; i++)
		free_irq(pinstance->hrrq_vector[i].vector,
			 &(pinstance->hrrq_vector[i]));

	if (pinstance->interrupt_mode) {
		pci_disable_msix(pinstance->pdev);
		pinstance->interrupt_mode = 0;
	}
}
