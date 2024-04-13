ssh_packet_send_ignore(struct ssh *ssh, int nbytes)
{
	u_int32_t rnd = 0;
	int r, i;

	if ((r = sshpkt_start(ssh, compat20 ?
	    SSH2_MSG_IGNORE : SSH_MSG_IGNORE)) != 0 ||
	    (r = sshpkt_put_u32(ssh, nbytes)) != 0)
		fatal("%s: %s", __func__, ssh_err(r));
	for (i = 0; i < nbytes; i++) {
		if (i % 4 == 0)
			rnd = arc4random();
		if ((r = sshpkt_put_u8(ssh, (u_char)rnd & 0xff)) != 0)
			fatal("%s: %s", __func__, ssh_err(r));
		rnd >>= 8;
	}
}
