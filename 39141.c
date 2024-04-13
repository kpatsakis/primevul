static void wake_up_sem_queue_do(struct list_head *pt)
{
	struct sem_queue *q, *t;
	int did_something;

	did_something = !list_empty(pt);
	list_for_each_entry_safe(q, t, pt, list) {
		wake_up_process(q->sleeper);
		/* q can disappear immediately after writing q->status. */
		smp_wmb();
		q->status = q->pid;
	}
	if (did_something)
		preempt_enable();
}
