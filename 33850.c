sshpkt_put_bignum1(struct ssh *ssh, const BIGNUM *v)
{
	return sshbuf_put_bignum1(ssh->state->outgoing_packet, v);
}
