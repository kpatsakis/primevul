ssh_packet_send_debug(struct ssh *ssh, const char *fmt,...)
{
	char buf[1024];
	va_list args;
	int r;

	if (compat20 && (ssh->compat & SSH_BUG_DEBUG))
		return;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	if (compat20) {
		if ((r = sshpkt_start(ssh, SSH2_MSG_DEBUG)) != 0 ||
		    (r = sshpkt_put_u8(ssh, 0)) != 0 || /* always display */
		    (r = sshpkt_put_cstring(ssh, buf)) != 0 ||
		    (r = sshpkt_put_cstring(ssh, "")) != 0 ||
		    (r = sshpkt_send(ssh)) != 0)
			fatal("%s: %s", __func__, ssh_err(r));
	} else {
		if ((r = sshpkt_start(ssh, SSH_MSG_DEBUG)) != 0 ||
		    (r = sshpkt_put_cstring(ssh, buf)) != 0 ||
		    (r = sshpkt_send(ssh)) != 0)
			fatal("%s: %s", __func__, ssh_err(r));
	}
	if ((r = ssh_packet_write_wait(ssh)) != 0)
		fatal("%s: %s", __func__, ssh_err(r));
}
