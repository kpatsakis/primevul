void *nlmsg_tail(const struct nlmsghdr *nlh)
{
	return (unsigned char *) nlh + NLMSG_ALIGN(nlh->nlmsg_len);
}
