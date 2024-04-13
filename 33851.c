sshpkt_put_bignum2(struct ssh *ssh, const BIGNUM *v)
{
	return sshbuf_put_bignum2(ssh->state->outgoing_packet, v);
}
