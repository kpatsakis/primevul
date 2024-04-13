sshpkt_put_u32(struct ssh *ssh, u_int32_t val)
{
	return sshbuf_put_u32(ssh->state->outgoing_packet, val);
}
