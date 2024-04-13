sshpkt_put_stringb(struct ssh *ssh, const struct sshbuf *v)
{
	return sshbuf_put_stringb(ssh->state->outgoing_packet, v);
}
