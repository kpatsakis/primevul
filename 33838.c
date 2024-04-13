sshpkt_disconnect(struct ssh *ssh, const char *fmt,...)
{
	char buf[1024];
	va_list args;
	int r;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	if (compat20) {
		if ((r = sshpkt_start(ssh, SSH2_MSG_DISCONNECT)) != 0 ||
		    (r = sshpkt_put_u32(ssh, SSH2_DISCONNECT_PROTOCOL_ERROR)) != 0 ||
		    (r = sshpkt_put_cstring(ssh, buf)) != 0 ||
		    (r = sshpkt_put_cstring(ssh, "")) != 0 ||
		    (r = sshpkt_send(ssh)) != 0)
			return r;
	} else {
		if ((r = sshpkt_start(ssh, SSH_MSG_DISCONNECT)) != 0 ||
		    (r = sshpkt_put_cstring(ssh, buf)) != 0 ||
		    (r = sshpkt_send(ssh)) != 0)
			return r;
	}
	return 0;
}
