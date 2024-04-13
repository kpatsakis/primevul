static void __mem_cgroup_free(struct mem_cgroup *memcg)
{
	int node;

	mem_cgroup_remove_from_trees(memcg);
	free_css_id(&mem_cgroup_subsys, &memcg->css);

	for_each_node(node)
		free_mem_cgroup_per_zone_info(memcg, node);

	free_percpu(memcg->stat);
	if (sizeof(struct mem_cgroup) < PAGE_SIZE)
		kfree(memcg);
	else
		vfree(memcg);
}
