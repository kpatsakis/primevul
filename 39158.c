static void ipc_schedule_free(struct rcu_head *head)
{
	struct ipc_rcu_grace *grace;
	struct ipc_rcu_sched *sched;

	grace = container_of(head, struct ipc_rcu_grace, rcu);
	sched = container_of(&(grace->data[0]), struct ipc_rcu_sched,
				data[0]);

	INIT_WORK(&sched->work, ipc_do_vfree);
	schedule_work(&sched->work);
}
