static inline struct timespec ep_set_mstimeout(long ms)
{
	struct timespec now, ts = {
		.tv_sec = ms / MSEC_PER_SEC,
		.tv_nsec = NSEC_PER_MSEC * (ms % MSEC_PER_SEC),
	};

	ktime_get_ts(&now);
	return timespec_add_safe(now, ts);
}
