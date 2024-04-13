sshpkt_get_u64(struct ssh *ssh, u_int64_t *valp)
{
	return sshbuf_get_u64(ssh->state->incoming_packet, valp);
}
