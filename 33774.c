void nlmsg_set_default_size(size_t max)
{
	if (max < nlmsg_total_size(0))
		max = nlmsg_total_size(0);

	default_msg_size = max;
}
