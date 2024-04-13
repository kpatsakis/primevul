static bool update_sd_pick_busiest(struct lb_env *env,
				   struct sd_lb_stats *sds,
				   struct sched_group *sg,
				   struct sg_lb_stats *sgs)
{
	struct sg_lb_stats *busiest = &sds->busiest_stat;

	/*
	 * Don't try to pull misfit tasks we can't help.
	 * We can use max_capacity here as reduction in capacity on some
	 * CPUs in the group should either be possible to resolve
	 * internally or be covered by avg_load imbalance (eventually).
	 */
	if (sgs->group_type == group_misfit_task &&
	    (!group_smaller_max_cpu_capacity(sg, sds->local) ||
	     !group_has_capacity(env, &sds->local_stat)))
		return false;

	if (sgs->group_type > busiest->group_type)
		return true;

	if (sgs->group_type < busiest->group_type)
		return false;

	if (sgs->avg_load <= busiest->avg_load)
		return false;

	if (!(env->sd->flags & SD_ASYM_CPUCAPACITY))
		goto asym_packing;

	/*
	 * Candidate sg has no more than one task per CPU and
	 * has higher per-CPU capacity. Migrating tasks to less
	 * capable CPUs may harm throughput. Maximize throughput,
	 * power/energy consequences are not considered.
	 */
	if (sgs->sum_nr_running <= sgs->group_weight &&
	    group_smaller_min_cpu_capacity(sds->local, sg))
		return false;

	/*
	 * If we have more than one misfit sg go with the biggest misfit.
	 */
	if (sgs->group_type == group_misfit_task &&
	    sgs->group_misfit_task_load < busiest->group_misfit_task_load)
		return false;

asym_packing:
	/* This is the busiest node in its class. */
	if (!(env->sd->flags & SD_ASYM_PACKING))
		return true;

	/* No ASYM_PACKING if target CPU is already busy */
	if (env->idle == CPU_NOT_IDLE)
		return true;
	/*
	 * ASYM_PACKING needs to move all the work to the highest
	 * prority CPUs in the group, therefore mark all groups
	 * of lower priority than ourself as busy.
	 */
	if (sgs->sum_nr_running &&
	    sched_asym_prefer(env->dst_cpu, sg->asym_prefer_cpu)) {
		if (!sds->busiest)
			return true;

		/* Prefer to move from lowest priority CPU's work */
		if (sched_asym_prefer(sds->busiest->asym_prefer_cpu,
				      sg->asym_prefer_cpu))
			return true;
	}

	return false;
}
