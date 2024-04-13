jiffies_to_timespec(const unsigned long jiffies, struct timespec *value)
{
	/*
 	 * Convert jiffies to nanoseconds and separate with
 	 * one divide.
 	 */
	u32 rem;
	value->tv_sec = div_u64_rem((u64)jiffies * TICK_NSEC,
				    NSEC_PER_SEC, &rem);
	value->tv_nsec = rem;
 }
