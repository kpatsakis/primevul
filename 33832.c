ssh_packet_start_compression(struct ssh *ssh, int level)
{
	int r;

	if (ssh->state->packet_compression && !compat20)
		return SSH_ERR_INTERNAL_ERROR;
	ssh->state->packet_compression = 1;
	if ((r = ssh_packet_init_compression(ssh)) != 0 ||
	    (r = start_compression_in(ssh)) != 0 ||
	    (r = start_compression_out(ssh, level)) != 0)
		return r;
	return 0;
}
