static void *print_genl_msg(struct nl_msg *msg, FILE *ofd, struct nlmsghdr *hdr,
			    struct nl_cache_ops *ops, int *payloadlen)
{
	void *data = nlmsg_data(hdr);

	if (*payloadlen < GENL_HDRLEN)
		return data;

	print_genl_hdr(ofd, data);

	*payloadlen -= GENL_HDRLEN;
	data += GENL_HDRLEN;

	if (ops) {
		int hdrsize = ops->co_hdrsize - GENL_HDRLEN;

		if (hdrsize > 0) {
			if (*payloadlen < hdrsize)
				return data;

			fprintf(ofd, "  [HEADER] %d octets\n", hdrsize);
			dump_hex(ofd, data, hdrsize, 0);

			*payloadlen -= hdrsize;
			data += hdrsize;
		}
	}

	return data;
}
