void posix_cpu_timers_exit_group(struct task_struct *tsk)
{
	cleanup_timers(tsk->signal->cpu_timers,
		       cputime_add(tsk->utime, tsk->signal->utime),
		       cputime_add(tsk->stime, tsk->signal->stime),
		     tsk->se.sum_exec_runtime + tsk->signal->sum_sched_runtime);
}
