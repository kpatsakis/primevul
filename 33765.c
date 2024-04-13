int nlmsg_get_proto(struct nl_msg *msg)
{
	return msg->nm_protocol;
}
