sshpkt_get_bignum2(struct ssh *ssh, BIGNUM *v)
{
	return sshbuf_get_bignum2(ssh->state->incoming_packet, v);
}
