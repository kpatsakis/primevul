static void igmp_gq_timer_expire(unsigned long data)
{
	struct in_device *in_dev = (struct in_device *)data;

	in_dev->mr_gq_running = 0;
	igmpv3_send_report(in_dev, NULL);
	__in_dev_put(in_dev);
}
