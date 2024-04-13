__mem_cgroup_commit_charge_swapin(struct page *page, struct mem_cgroup *memcg,
					enum charge_type ctype)
{
	if (mem_cgroup_disabled())
		return;
	if (!memcg)
		return;
	cgroup_exclude_rmdir(&memcg->css);

	__mem_cgroup_commit_charge_lrucare(page, memcg, ctype);
	/*
	 * Now swap is on-memory. This means this page may be
	 * counted both as mem and swap....double count.
	 * Fix it by uncharging from memsw. Basically, this SwapCache is stable
	 * under lock_page(). But in do_swap_page()::memory.c, reuse_swap_page()
	 * may call delete_from_swap_cache() before reach here.
	 */
	if (do_swap_account && PageSwapCache(page)) {
		swp_entry_t ent = {.val = page_private(page)};
		struct mem_cgroup *swap_memcg;
		unsigned short id;

		id = swap_cgroup_record(ent, 0);
		rcu_read_lock();
		swap_memcg = mem_cgroup_lookup(id);
		if (swap_memcg) {
			/*
			 * This recorded memcg can be obsolete one. So, avoid
			 * calling css_tryget
			 */
			if (!mem_cgroup_is_root(swap_memcg))
				res_counter_uncharge(&swap_memcg->memsw,
						     PAGE_SIZE);
			mem_cgroup_swap_statistics(swap_memcg, false);
			mem_cgroup_put(swap_memcg);
		}
		rcu_read_unlock();
	}
	/*
	 * At swapin, we may charge account against cgroup which has no tasks.
	 * So, rmdir()->pre_destroy() can be called while we do this charge.
	 * In that case, we need to call pre_destroy() again. check it here.
	 */
	cgroup_release_and_wakeup_rmdir(&memcg->css);
}
