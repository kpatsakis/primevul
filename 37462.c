pmcraid_release_cmd_blocks(struct pmcraid_instance *pinstance, int max_index)
{
	int i;
	for (i = 0; i < max_index; i++) {
		kmem_cache_free(pinstance->cmd_cachep, pinstance->cmd_list[i]);
		pinstance->cmd_list[i] = NULL;
	}
	kmem_cache_destroy(pinstance->cmd_cachep);
	pinstance->cmd_cachep = NULL;
}
