void mem_cgroup_out_of_memory(struct mem_cgroup *mem, gfp_t gfp_mask)
{
	unsigned long limit;
	unsigned int points = 0;
	struct task_struct *p;

	/*
	 * If current has a pending SIGKILL, then automatically select it.  The
	 * goal is to allow it to allocate so that it may quickly exit and free
	 * its memory.
	 */
	if (fatal_signal_pending(current)) {
		set_thread_flag(TIF_MEMDIE);
		return;
	}

	check_panic_on_oom(CONSTRAINT_MEMCG, gfp_mask, 0, NULL);
	limit = mem_cgroup_get_limit(mem) >> PAGE_SHIFT;
	read_lock(&tasklist_lock);
retry:
	p = select_bad_process(&points, limit, mem, NULL);
	if (!p || PTR_ERR(p) == -1UL)
		goto out;

	if (oom_kill_process(p, gfp_mask, 0, points, limit, mem, NULL,
				"Memory cgroup out of memory"))
		goto retry;
out:
	read_unlock(&tasklist_lock);
}
