sshpkt_get_ec(struct ssh *ssh, EC_POINT *v, const EC_GROUP *g)
{
	return sshbuf_get_ec(ssh->state->incoming_packet, v, g);
}
