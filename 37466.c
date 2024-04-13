pmcraid_release_host_rrqs(struct pmcraid_instance *pinstance, int maxindex)
{
	int i;
	for (i = 0; i < maxindex; i++) {

		pci_free_consistent(pinstance->pdev,
				    HRRQ_ENTRY_SIZE * PMCRAID_MAX_CMD,
				    pinstance->hrrq_start[i],
				    pinstance->hrrq_start_bus_addr[i]);

		/* reset pointers and toggle bit to zeros */
		pinstance->hrrq_start[i] = NULL;
		pinstance->hrrq_start_bus_addr[i] = 0;
		pinstance->host_toggle_bit[i] = 0;
	}
}
