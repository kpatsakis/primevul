static void igmp_gq_start_timer(struct in_device *in_dev)
{
	int tv = net_random() % in_dev->mr_maxdelay;

	in_dev->mr_gq_running = 1;
	if (!mod_timer(&in_dev->mr_gq_timer, jiffies+tv+2))
		in_dev_hold(in_dev);
}
