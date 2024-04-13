int nlmsg_valid_hdr(const struct nlmsghdr *nlh, int hdrlen)
{
	if (nlh->nlmsg_len < nlmsg_msg_size(hdrlen))
		return 0;

	return 1;
}
