int do_sys_settimeofday(struct timespec *tv, struct timezone *tz)
{
	static int firsttime = 1;
	int error = 0;

	if (tv && !timespec_valid(tv))
		return -EINVAL;

	error = security_settime(tv, tz);
	if (error)
		return error;

	if (tz) {
		/* SMP safe, global irq locking makes it work. */
		sys_tz = *tz;
		update_vsyscall_tz();
		if (firsttime) {
			firsttime = 0;
			if (!tv)
				warp_clock();
		}
	}
	if (tv)
	{
		/* SMP safe, again the code in arch/foo/time.c should
		 * globally block out interrupts when it runs.
		 */
		return do_settimeofday(tv);
	}
	return 0;
}
