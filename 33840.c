sshpkt_get(struct ssh *ssh, void *valp, size_t len)
{
	return sshbuf_get(ssh->state->incoming_packet, valp, len);
}
