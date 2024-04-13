ssh_packet_get_bytes(struct ssh *ssh, u_int64_t *ibytes, u_int64_t *obytes)
{
	if (ibytes)
		*ibytes = ssh->state->p_read.bytes;
	if (obytes)
		*obytes = ssh->state->p_send.bytes;
}
