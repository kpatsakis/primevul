static int xtables_lock(int wait, struct timeval *wait_interval)
{
	struct timeval time_left, wait_time;
	int fd, i = 0;

	time_left.tv_sec = wait;
	time_left.tv_usec = 0;

	fd = open(XT_LOCK_NAME, O_CREAT, 0600);
	if (fd < 0) {
		fprintf(stderr, "Fatal: can't open lock file %s: %s\n",
			XT_LOCK_NAME, strerror(errno));
		return XT_LOCK_FAILED;
	}

	if (wait == -1) {
		if (flock(fd, LOCK_EX) == 0)
			return fd;

		fprintf(stderr, "Can't lock %s: %s\n", XT_LOCK_NAME,
			strerror(errno));
		return XT_LOCK_BUSY;
	}

	while (1) {
		if (flock(fd, LOCK_EX | LOCK_NB) == 0)
			return fd;
		else if (timercmp(&time_left, wait_interval, <))
			return XT_LOCK_BUSY;

		if (++i % 10 == 0) {
			fprintf(stderr, "Another app is currently holding the xtables lock; "
				"still %lds %ldus time ahead to have a chance to grab the lock...\n",
				time_left.tv_sec, time_left.tv_usec);
		}

		wait_time = *wait_interval;
		select(0, NULL, NULL, NULL, &wait_time);
		timersub(&time_left, wait_interval, &time_left);
	}
}
