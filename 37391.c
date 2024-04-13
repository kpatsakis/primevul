pmcraid_allocate_control_blocks(struct pmcraid_instance *pinstance)
{
	int i;

	sprintf(pinstance->ctl_pool_name, "pmcraid_control_pool_%d",
		pinstance->host->unique_id);

	pinstance->control_pool =
		pci_pool_create(pinstance->ctl_pool_name,
				pinstance->pdev,
				sizeof(struct pmcraid_control_block),
				PMCRAID_IOARCB_ALIGNMENT, 0);

	if (!pinstance->control_pool)
		return -ENOMEM;

	for (i = 0; i < PMCRAID_MAX_CMD; i++) {
		pinstance->cmd_list[i]->ioa_cb =
			pci_pool_alloc(
				pinstance->control_pool,
				GFP_KERNEL,
				&(pinstance->cmd_list[i]->ioa_cb_bus_addr));

		if (!pinstance->cmd_list[i]->ioa_cb) {
			pmcraid_release_control_blocks(pinstance, i);
			return -ENOMEM;
		}
		memset(pinstance->cmd_list[i]->ioa_cb, 0,
			sizeof(struct pmcraid_control_block));
	}
	return 0;
}
