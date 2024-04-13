static void dump_error_msg(struct nl_msg *msg, FILE *ofd)
{
	struct nlmsghdr *hdr = nlmsg_hdr(msg);
	struct nlmsgerr *err = nlmsg_data(hdr);

	fprintf(ofd, "  [ERRORMSG] %zu octets\n", sizeof(*err));

	if (nlmsg_len(hdr) >= sizeof(*err)) {
		struct nl_msg *errmsg;

		fprintf(ofd, "    .error = %d \"%s\"\n", err->error,
			nl_strerror_l(-err->error));
		fprintf(ofd, "  [ORIGINAL MESSAGE] %zu octets\n", sizeof(*hdr));

		errmsg = nlmsg_inherit(&err->msg);
		print_hdr(ofd, errmsg);
		nlmsg_free(errmsg);
	}
}
