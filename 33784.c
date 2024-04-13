static void print_genl_hdr(FILE *ofd, void *start)
{
	struct genlmsghdr *ghdr = start;

	fprintf(ofd, "  [GENERIC NETLINK HEADER] %zu octets\n", GENL_HDRLEN);
	fprintf(ofd, "    .cmd = %u\n", ghdr->cmd);
	fprintf(ofd, "    .version = %u\n", ghdr->version);
	fprintf(ofd, "    .unused = %#x\n", ghdr->reserved);
}
