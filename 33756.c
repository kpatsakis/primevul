struct nl_msg *nlmsg_convert(struct nlmsghdr *hdr)
{
	struct nl_msg *nm;

	nm = __nlmsg_alloc(NLMSG_ALIGN(hdr->nlmsg_len));
	if (!nm)
		return NULL;

	memcpy(nm->nm_nlh, hdr, hdr->nlmsg_len);

	return nm;
}
