pmcraid_allocate_cmd_blocks(struct pmcraid_instance *pinstance)
{
	int i;

	sprintf(pinstance->cmd_pool_name, "pmcraid_cmd_pool_%d",
		pinstance->host->unique_id);


	pinstance->cmd_cachep = kmem_cache_create(
					pinstance->cmd_pool_name,
					sizeof(struct pmcraid_cmd), 0,
					SLAB_HWCACHE_ALIGN, NULL);
	if (!pinstance->cmd_cachep)
		return -ENOMEM;

	for (i = 0; i < PMCRAID_MAX_CMD; i++) {
		pinstance->cmd_list[i] =
			kmem_cache_alloc(pinstance->cmd_cachep, GFP_KERNEL);
		if (!pinstance->cmd_list[i]) {
			pmcraid_release_cmd_blocks(pinstance, i);
			return -ENOMEM;
		}
	}
	return 0;
}
