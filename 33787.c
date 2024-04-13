static void print_msg(struct nl_msg *msg, FILE *ofd, struct nlmsghdr *hdr)
{
	struct nl_cache_ops *ops;
	int payloadlen = nlmsg_len(hdr);
	int attrlen = 0;
	void *data;

	data = nlmsg_data(hdr);
	ops = nl_cache_ops_associate_safe(nlmsg_get_proto(msg),
					  hdr->nlmsg_type);
	if (ops) {
		attrlen = nlmsg_attrlen(hdr, ops->co_hdrsize);
		payloadlen -= attrlen;
	}

	if (msg->nm_protocol == NETLINK_GENERIC)
		data = print_genl_msg(msg, ofd, hdr, ops, &payloadlen);

	if (payloadlen) {
		fprintf(ofd, "  [PAYLOAD] %d octets\n", payloadlen);
		dump_hex(ofd, data, payloadlen, 0);
	}

	if (attrlen) {
		struct nlattr *attrs;
		int attrlen;
		
		attrs = nlmsg_attrdata(hdr, ops->co_hdrsize);
		attrlen = nlmsg_attrlen(hdr, ops->co_hdrsize);
		dump_attrs(ofd, attrs, attrlen, 0);
	}

	if (ops)
		nl_cache_ops_put(ops);
}
