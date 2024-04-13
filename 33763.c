struct sockaddr_nl *nlmsg_get_dst(struct nl_msg *msg)
{
	return &msg->nm_dst;
}
