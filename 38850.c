static void defense_work_handler(struct work_struct *work)
{
	update_defense_level();
	if (atomic_read(&ip_vs_dropentry))
		ip_vs_random_dropentry();

	schedule_delayed_work(&defense_work, DEFENSE_TIMER_PERIOD);
}
