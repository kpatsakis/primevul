sshpkt_putb(struct ssh *ssh, const struct sshbuf *b)
{
	return sshbuf_putb(ssh->state->outgoing_packet, b);
}
