size_t nlmsg_get_max_size(struct nl_msg *msg)
{
	return msg->nm_size;
}
