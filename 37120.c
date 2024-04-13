static void notify_cmos_timer(void)
{
	if (!no_sync_cmos_clock)
		mod_timer(&sync_cmos_timer, jiffies + 1);
}
