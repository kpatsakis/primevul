sshpkt_put_cstring(struct ssh *ssh, const void *v)
{
	return sshbuf_put_cstring(ssh->state->outgoing_packet, v);
}
