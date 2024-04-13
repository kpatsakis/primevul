ssh_packet_read_expect(struct ssh *ssh, u_int expected_type)
{
	int r;
	u_char type;

	if ((r = ssh_packet_read_seqnr(ssh, &type, NULL)) != 0)
		return r;
	if (type != expected_type) {
		if ((r = sshpkt_disconnect(ssh,
		    "Protocol error: expected packet type %d, got %d",
		    expected_type, type)) != 0)
			return r;
		return SSH_ERR_PROTOCOL_ERROR;
	}
	return 0;
}
