static void pmcraid_release_config_buffers(struct pmcraid_instance *pinstance)
{
	if (pinstance->cfg_table != NULL &&
	    pinstance->cfg_table_bus_addr != 0) {
		pci_free_consistent(pinstance->pdev,
				    sizeof(struct pmcraid_config_table),
				    pinstance->cfg_table,
				    pinstance->cfg_table_bus_addr);
		pinstance->cfg_table = NULL;
		pinstance->cfg_table_bus_addr = 0;
	}

	if (pinstance->res_entries != NULL) {
		int i;

		for (i = 0; i < PMCRAID_MAX_RESOURCES; i++)
			list_del(&pinstance->res_entries[i].queue);
		kfree(pinstance->res_entries);
		pinstance->res_entries = NULL;
	}

	pmcraid_release_hcams(pinstance);
}
