void mem_cgroup_reset_owner(struct page *newpage)
{
	struct page_cgroup *pc;

	if (mem_cgroup_disabled())
		return;

	pc = lookup_page_cgroup(newpage);
	VM_BUG_ON(PageCgroupUsed(pc));
	pc->mem_cgroup = root_mem_cgroup;
}
