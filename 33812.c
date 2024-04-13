ssh_packet_read(struct ssh *ssh)
{
	u_char type;
	int r;

	if ((r = ssh_packet_read_seqnr(ssh, &type, NULL)) != 0)
		fatal("%s: %s", __func__, ssh_err(r));
	return type;
}
