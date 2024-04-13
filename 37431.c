static int __devinit pmcraid_init_buffers(struct pmcraid_instance *pinstance)
{
	int i;

	if (pmcraid_allocate_host_rrqs(pinstance)) {
		pmcraid_err("couldn't allocate memory for %d host rrqs\n",
			     pinstance->num_hrrq);
		return -ENOMEM;
	}

	if (pmcraid_allocate_config_buffers(pinstance)) {
		pmcraid_err("couldn't allocate memory for config buffers\n");
		pmcraid_release_host_rrqs(pinstance, pinstance->num_hrrq);
		return -ENOMEM;
	}

	if (pmcraid_allocate_cmd_blocks(pinstance)) {
		pmcraid_err("couldn't allocate memory for cmd blocks\n");
		pmcraid_release_config_buffers(pinstance);
		pmcraid_release_host_rrqs(pinstance, pinstance->num_hrrq);
		return -ENOMEM;
	}

	if (pmcraid_allocate_control_blocks(pinstance)) {
		pmcraid_err("couldn't allocate memory control blocks\n");
		pmcraid_release_config_buffers(pinstance);
		pmcraid_release_cmd_blocks(pinstance, PMCRAID_MAX_CMD);
		pmcraid_release_host_rrqs(pinstance, pinstance->num_hrrq);
		return -ENOMEM;
	}

	/* allocate DMAable memory for page D0 INQUIRY buffer */
	pinstance->inq_data = pci_alloc_consistent(
					pinstance->pdev,
					sizeof(struct pmcraid_inquiry_data),
					&pinstance->inq_data_baddr);

	if (pinstance->inq_data == NULL) {
		pmcraid_err("couldn't allocate DMA memory for INQUIRY\n");
		pmcraid_release_buffers(pinstance);
		return -ENOMEM;
	}

	/* allocate DMAable memory for set timestamp data buffer */
	pinstance->timestamp_data = pci_alloc_consistent(
					pinstance->pdev,
					sizeof(struct pmcraid_timestamp_data),
					&pinstance->timestamp_data_baddr);

	if (pinstance->timestamp_data == NULL) {
		pmcraid_err("couldn't allocate DMA memory for \
				set time_stamp \n");
		pmcraid_release_buffers(pinstance);
		return -ENOMEM;
	}


	/* Initialize all the command blocks and add them to free pool. No
	 * need to lock (free_pool_lock) as this is done in initialization
	 * itself
	 */
	for (i = 0; i < PMCRAID_MAX_CMD; i++) {
		struct pmcraid_cmd *cmdp = pinstance->cmd_list[i];
		pmcraid_init_cmdblk(cmdp, i);
		cmdp->drv_inst = pinstance;
		list_add_tail(&cmdp->free_list, &pinstance->free_cmd_pool);
	}

	return 0;
}
