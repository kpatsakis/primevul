static void ntp_update_frequency(void)
{
	u64 second_length = (u64)(tick_usec * NSEC_PER_USEC * USER_HZ)
				<< TICK_LENGTH_SHIFT;
	second_length += (s64)ntp_tick_adj << TICK_LENGTH_SHIFT;
	second_length += (s64)time_freq << (TICK_LENGTH_SHIFT - SHIFT_NSEC);

	tick_length_base = second_length;

	tick_nsec = div_u64(second_length, HZ) >> TICK_LENGTH_SHIFT;
	tick_length_base = div_u64(tick_length_base, NTP_INTERVAL_FREQ);
}
