void mem_cgroup_replace_page_cache(struct page *oldpage,
				  struct page *newpage)
{
	struct mem_cgroup *memcg;
	struct page_cgroup *pc;
	enum charge_type type = MEM_CGROUP_CHARGE_TYPE_CACHE;

	if (mem_cgroup_disabled())
		return;

	pc = lookup_page_cgroup(oldpage);
	/* fix accounting on old pages */
	lock_page_cgroup(pc);
	memcg = pc->mem_cgroup;
	mem_cgroup_charge_statistics(memcg, PageCgroupCache(pc), -1);
	ClearPageCgroupUsed(pc);
	unlock_page_cgroup(pc);

	if (PageSwapBacked(oldpage))
		type = MEM_CGROUP_CHARGE_TYPE_SHMEM;

	/*
	 * Even if newpage->mapping was NULL before starting replacement,
	 * the newpage may be on LRU(or pagevec for LRU) already. We lock
	 * LRU while we overwrite pc->mem_cgroup.
	 */
	__mem_cgroup_commit_charge_lrucare(newpage, memcg, type);
}
