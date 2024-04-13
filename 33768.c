static int nlmsg_len(const struct nlmsghdr *nlh)
{
	return nlmsg_datalen(nlh);
}
