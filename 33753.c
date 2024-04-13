struct nl_msg *nlmsg_alloc_size(size_t max)
{
	return __nlmsg_alloc(max);
}
