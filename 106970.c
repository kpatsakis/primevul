static void set_loadctl_defaults(void)
{
	struct rlimit rlim;

	/* Workers need to up 'em, master needs to know 'em */
	getrlimit(RLIMIT_NOFILE, &rlim);
	rlim.rlim_cur = rlim.rlim_max;
	setrlimit(RLIMIT_NOFILE, &rlim);
	loadctl.nofile_limit = rlim.rlim_max;
#ifdef RLIMIT_NPROC
	getrlimit(RLIMIT_NPROC, &rlim);
	rlim.rlim_cur = rlim.rlim_max;
	setrlimit(RLIMIT_NPROC, &rlim);
	loadctl.nproc_limit = rlim.rlim_max;
#else
	loadctl.nproc_limit = loadctl.nofile_limit / 2;
#endif

	/*
	 * things may have been configured already. Otherwise we
	 * set some sort of sane defaults here
	 */
	if (!loadctl.jobs_max) {
		loadctl.jobs_max = loadctl.nproc_limit - 100;
		if (!is_worker && loadctl.jobs_max > (loadctl.nofile_limit - 50) * wproc_num_workers_online) {
			loadctl.jobs_max = (loadctl.nofile_limit - 50) * wproc_num_workers_online;
		}
	}

	if (!loadctl.jobs_limit)
		loadctl.jobs_limit = loadctl.jobs_max;

	if (!loadctl.backoff_limit)
		loadctl.backoff_limit = online_cpus() * 2.5;
	if (!loadctl.rampup_limit)
		loadctl.rampup_limit = online_cpus() * 0.8;
	if (!loadctl.backoff_change)
		loadctl.backoff_change = loadctl.jobs_limit * 0.3;
	if (!loadctl.rampup_change)
		loadctl.rampup_change = loadctl.backoff_change * 0.25;
	if (!loadctl.check_interval)
		loadctl.check_interval = 60;
	if (!loadctl.jobs_min)
		loadctl.jobs_min = online_cpus() * 20; /* pessimistic */
}
