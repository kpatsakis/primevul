sshpkt_ptr(struct ssh *ssh, size_t *lenp)
{
	if (lenp != NULL)
		*lenp = sshbuf_len(ssh->state->incoming_packet);
	return sshbuf_ptr(ssh->state->incoming_packet);
}
