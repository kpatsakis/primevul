void nlmsg_set_dst(struct nl_msg *msg, struct sockaddr_nl *addr)
{
	memcpy(&msg->nm_dst, addr, sizeof(*addr));
}
