int nl_msg_parse(struct nl_msg *msg, void (*cb)(struct nl_object *, void *),
		 void *arg)
{
	struct nl_cache_ops *ops;
	struct nl_parser_param p = {
		.pp_cb = parse_cb
	};
	struct dp_xdata x = {
		.cb = cb,
		.arg = arg,
	};
	int err;

	ops = nl_cache_ops_associate_safe(nlmsg_get_proto(msg),
					  nlmsg_hdr(msg)->nlmsg_type);
	if (ops == NULL)
		return -NLE_MSGTYPE_NOSUPPORT;
	p.pp_arg = &x;

	err = nl_cache_parse(ops, NULL, nlmsg_hdr(msg), &p);
	nl_cache_ops_put(ops);

	return err;
}
