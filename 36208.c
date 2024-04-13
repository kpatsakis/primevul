static void igmp_ifc_start_timer(struct in_device *in_dev, int delay)
{
	int tv = net_random() % delay;

	if (!mod_timer(&in_dev->mr_ifc_timer, jiffies+tv+2))
		in_dev_hold(in_dev);
}
