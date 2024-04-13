static void print_hdr(FILE *ofd, struct nl_msg *msg)
{
	struct nlmsghdr *nlh = nlmsg_hdr(msg);
	struct nl_cache_ops *ops;
	struct nl_msgtype *mt;
	char buf[128];

	fprintf(ofd, "    .nlmsg_len = %d\n", nlh->nlmsg_len);

	ops = nl_cache_ops_associate_safe(nlmsg_get_proto(msg), nlh->nlmsg_type);
	if (ops) {
		mt = nl_msgtype_lookup(ops, nlh->nlmsg_type);
		if (!mt)
			BUG();

		snprintf(buf, sizeof(buf), "%s::%s", ops->co_name, mt->mt_name);
		nl_cache_ops_put(ops);
	} else
		nl_nlmsgtype2str(nlh->nlmsg_type, buf, sizeof(buf));

	fprintf(ofd, "    .type = %d <%s>\n", nlh->nlmsg_type, buf);
	fprintf(ofd, "    .flags = %d <%s>\n", nlh->nlmsg_flags,
		nl_nlmsg_flags2str(nlh->nlmsg_flags, buf, sizeof(buf)));
	fprintf(ofd, "    .seq = %d\n", nlh->nlmsg_seq);
	fprintf(ofd, "    .port = %d\n", nlh->nlmsg_pid);

}
