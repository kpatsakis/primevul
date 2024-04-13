static int mem_cgroup_charge_common(struct page *page, struct mm_struct *mm,
				gfp_t gfp_mask, enum charge_type ctype)
{
	struct mem_cgroup *memcg = NULL;
	unsigned int nr_pages = 1;
	struct page_cgroup *pc;
	bool oom = true;
	int ret;

	if (PageTransHuge(page)) {
		nr_pages <<= compound_order(page);
		VM_BUG_ON(!PageTransHuge(page));
		/*
		 * Never OOM-kill a process for a huge page.  The
		 * fault handler will fall back to regular pages.
		 */
		oom = false;
	}

	pc = lookup_page_cgroup(page);
	ret = __mem_cgroup_try_charge(mm, gfp_mask, nr_pages, &memcg, oom);
	if (ret == -ENOMEM)
		return ret;
	__mem_cgroup_commit_charge(memcg, page, nr_pages, pc, ctype);
	return 0;
}
