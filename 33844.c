sshpkt_get_string(struct ssh *ssh, u_char **valp, size_t *lenp)
{
	return sshbuf_get_string(ssh->state->incoming_packet, valp, lenp);
}
