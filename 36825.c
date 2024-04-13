static struct task_struct *select_bad_process(unsigned int *ppoints,
		unsigned long totalpages, struct mem_cgroup *mem,
		const nodemask_t *nodemask)
{
	struct task_struct *g, *p;
	struct task_struct *chosen = NULL;
	*ppoints = 0;

	do_each_thread(g, p) {
		unsigned int points;

		if (p->exit_state)
			continue;
		if (oom_unkillable_task(p, mem, nodemask))
			continue;

		/*
		 * This task already has access to memory reserves and is
		 * being killed. Don't allow any other task access to the
		 * memory reserve.
		 *
		 * Note: this may have a chance of deadlock if it gets
		 * blocked waiting for another task which itself is waiting
		 * for memory. Is there a better alternative?
		 */
		if (test_tsk_thread_flag(p, TIF_MEMDIE))
			return ERR_PTR(-1UL);
		if (!p->mm)
			continue;

		if (p->flags & PF_EXITING) {
			/*
			 * If p is the current task and is in the process of
			 * releasing memory, we allow the "kill" to set
			 * TIF_MEMDIE, which will allow it to gain access to
			 * memory reserves.  Otherwise, it may stall forever.
			 *
			 * The loop isn't broken here, however, in case other
			 * threads are found to have already been oom killed.
			 */
			if (p == current) {
				chosen = p;
				*ppoints = 1000;
			} else {
				/*
				 * If this task is not being ptraced on exit,
				 * then wait for it to finish before killing
				 * some other task unnecessarily.
				 */
				if (!(p->group_leader->ptrace & PT_TRACE_EXIT))
					return ERR_PTR(-1UL);
			}
		}

		points = oom_badness(p, mem, nodemask, totalpages);
		if (points > *ppoints) {
			chosen = p;
			*ppoints = points;
		}
	} while_each_thread(g, p);

	return chosen;
}
