pmcraid_allocate_config_buffers(struct pmcraid_instance *pinstance)
{
	int i;

	pinstance->res_entries =
			kzalloc(sizeof(struct pmcraid_resource_entry) *
				PMCRAID_MAX_RESOURCES, GFP_KERNEL);

	if (NULL == pinstance->res_entries) {
		pmcraid_err("failed to allocate memory for resource table\n");
		return -ENOMEM;
	}

	for (i = 0; i < PMCRAID_MAX_RESOURCES; i++)
		list_add_tail(&pinstance->res_entries[i].queue,
			      &pinstance->free_res_q);

	pinstance->cfg_table =
		pci_alloc_consistent(pinstance->pdev,
				     sizeof(struct pmcraid_config_table),
				     &pinstance->cfg_table_bus_addr);

	if (NULL == pinstance->cfg_table) {
		pmcraid_err("couldn't alloc DMA memory for config table\n");
		pmcraid_release_config_buffers(pinstance);
		return -ENOMEM;
	}

	if (pmcraid_allocate_hcams(pinstance)) {
		pmcraid_err("could not alloc DMA memory for HCAMS\n");
		pmcraid_release_config_buffers(pinstance);
		return -ENOMEM;
	}

	return 0;
}
