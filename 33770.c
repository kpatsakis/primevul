int nlmsg_ok(const struct nlmsghdr *nlh, int remaining)
{
	return (remaining >= (int)sizeof(struct nlmsghdr) &&
		nlh->nlmsg_len >= sizeof(struct nlmsghdr) &&
		nlh->nlmsg_len <= remaining);
}
