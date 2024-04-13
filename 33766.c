struct sockaddr_nl *nlmsg_get_src(struct nl_msg *msg)
{
	return &msg->nm_src;
}
