struct nl_msg *nlmsg_alloc(void)
{
	return __nlmsg_alloc(default_msg_size);
}
