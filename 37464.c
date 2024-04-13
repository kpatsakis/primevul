pmcraid_release_control_blocks(
	struct pmcraid_instance *pinstance,
	int max_index
)
{
	int i;

	if (pinstance->control_pool == NULL)
		return;

	for (i = 0; i < max_index; i++) {
		pci_pool_free(pinstance->control_pool,
			      pinstance->cmd_list[i]->ioa_cb,
			      pinstance->cmd_list[i]->ioa_cb_bus_addr);
		pinstance->cmd_list[i]->ioa_cb = NULL;
		pinstance->cmd_list[i]->ioa_cb_bus_addr = 0;
	}
	pci_pool_destroy(pinstance->control_pool);
	pinstance->control_pool = NULL;
}
