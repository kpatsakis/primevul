static void pmcraid_kill_tasklets(struct pmcraid_instance *pinstance)
{
	int i;
	for (i = 0; i < pinstance->num_hrrq; i++)
		tasklet_kill(&pinstance->isr_tasklet[i]);
}
