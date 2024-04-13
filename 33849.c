sshpkt_put(struct ssh *ssh, const void *v, size_t len)
{
	return sshbuf_put(ssh->state->outgoing_packet, v, len);
}
