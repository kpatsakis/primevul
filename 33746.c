void nl_msg_dump(struct nl_msg *msg, FILE *ofd)
{
	struct nlmsghdr *hdr = nlmsg_hdr(msg);
	
	fprintf(ofd, 
	"--------------------------   BEGIN NETLINK MESSAGE ---------------------------\n");

	fprintf(ofd, "  [NETLINK HEADER] %zu octets\n", sizeof(struct nlmsghdr));
	print_hdr(ofd, msg);

	if (hdr->nlmsg_type == NLMSG_ERROR)
		dump_error_msg(msg, ofd);
	else if (nlmsg_len(hdr) > 0)
		print_msg(msg, ofd, hdr);

	fprintf(ofd, 
	"---------------------------  END NETLINK MESSAGE   ---------------------------\n");
}
