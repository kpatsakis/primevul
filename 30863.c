int xtables_lock_or_exit(int wait, struct timeval *wait_interval)
{
	int lock = xtables_lock(wait, wait_interval);

	if (lock == XT_LOCK_FAILED) {
		xtables_free_opts(1);
		exit(RESOURCE_PROBLEM);
	}

	if (lock == XT_LOCK_BUSY) {
		fprintf(stderr, "Another app is currently holding the xtables lock. ");
		if (wait == 0)
			fprintf(stderr, "Perhaps you want to use the -w option?\n");
		else
			fprintf(stderr, "Stopped waiting after %ds.\n", wait);
		xtables_free_opts(1);
		exit(RESOURCE_PROBLEM);
	}

	return lock;
}
