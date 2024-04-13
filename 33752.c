struct nl_msg *nlmsg_alloc_simple(int nlmsgtype, int flags)
{
	struct nl_msg *msg;
	struct nlmsghdr nlh = {
		.nlmsg_type = nlmsgtype,
		.nlmsg_flags = flags,
	};

	msg = nlmsg_inherit(&nlh);
	if (msg)
		NL_DBG(2, "msg %p: Allocated new simple message\n", msg);

	return msg;
}
