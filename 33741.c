static struct nl_msg *__nlmsg_alloc(size_t len)
{
	struct nl_msg *nm;

	if (len < sizeof(struct nlmsghdr))
		len = sizeof(struct nlmsghdr);

	nm = calloc(1, sizeof(*nm));
	if (!nm)
		goto errout;

	nm->nm_refcnt = 1;

	nm->nm_nlh = calloc(1, len);
	if (!nm->nm_nlh)
		goto errout;

	nm->nm_protocol = -1;
	nm->nm_size = len;
	nm->nm_nlh->nlmsg_len = nlmsg_total_size(0);

	NL_DBG(2, "msg %p: Allocated new message, maxlen=%zu\n", nm, len);

	return nm;
errout:
	free(nm);
	return NULL;
}
