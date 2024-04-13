static inline void warp_clock(void)
{
	write_seqlock_irq(&xtime_lock);
	wall_to_monotonic.tv_sec -= sys_tz.tz_minuteswest * 60;
	xtime.tv_sec += sys_tz.tz_minuteswest * 60;
	update_xtime_cache(0);
	write_sequnlock_irq(&xtime_lock);
	clock_was_set();
}
