sshpkt_put_u64(struct ssh *ssh, u_int64_t val)
{
	return sshbuf_put_u64(ssh->state->outgoing_packet, val);
}
