pmcraid_allocate_host_rrqs(struct pmcraid_instance *pinstance)
{
	int i, buffer_size;

	buffer_size = HRRQ_ENTRY_SIZE * PMCRAID_MAX_CMD;

	for (i = 0; i < pinstance->num_hrrq; i++) {
		pinstance->hrrq_start[i] =
			pci_alloc_consistent(
					pinstance->pdev,
					buffer_size,
					&(pinstance->hrrq_start_bus_addr[i]));

		if (pinstance->hrrq_start[i] == 0) {
			pmcraid_err("pci_alloc failed for hrrq vector : %d\n",
				    i);
			pmcraid_release_host_rrqs(pinstance, i);
			return -ENOMEM;
		}

		memset(pinstance->hrrq_start[i], 0, buffer_size);
		pinstance->hrrq_curr[i] = pinstance->hrrq_start[i];
		pinstance->hrrq_end[i] =
			pinstance->hrrq_start[i] + PMCRAID_MAX_CMD - 1;
		pinstance->host_toggle_bit[i] = 1;
		spin_lock_init(&pinstance->hrrq_lock[i]);
	}
	return 0;
}
