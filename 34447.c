auth_input_request_forwarding(struct passwd * pw)
{
	Channel *nc;
	int sock = -1;

	if (auth_sock_name != NULL) {
		error("authentication forwarding requested twice.");
		return 0;
	}

	/* Temporarily drop privileged uid for mkdir/bind. */
	temporarily_use_uid(pw);

	/* Allocate a buffer for the socket name, and format the name. */
	auth_sock_dir = xstrdup("/tmp/ssh-XXXXXXXXXX");

	/* Create private directory for socket */
	if (mkdtemp(auth_sock_dir) == NULL) {
		packet_send_debug("Agent forwarding disabled: "
		    "mkdtemp() failed: %.100s", strerror(errno));
		restore_uid();
		free(auth_sock_dir);
		auth_sock_dir = NULL;
		goto authsock_err;
	}

	xasprintf(&auth_sock_name, "%s/agent.%ld",
	    auth_sock_dir, (long) getpid());

	/* Start a Unix listener on auth_sock_name. */
	sock = unix_listener(auth_sock_name, SSH_LISTEN_BACKLOG, 0);

	/* Restore the privileged uid. */
	restore_uid();

	/* Check for socket/bind/listen failure. */
	if (sock < 0)
		goto authsock_err;

	/* Allocate a channel for the authentication agent socket. */
	nc = channel_new("auth socket",
	    SSH_CHANNEL_AUTH_SOCKET, sock, sock, -1,
	    CHAN_X11_WINDOW_DEFAULT, CHAN_X11_PACKET_DEFAULT,
	    0, "auth socket", 1);
	nc->path = xstrdup(auth_sock_name);
	return 1;

 authsock_err:
	free(auth_sock_name);
	if (auth_sock_dir != NULL) {
		rmdir(auth_sock_dir);
		free(auth_sock_dir);
	}
	if (sock != -1)
		close(sock);
	auth_sock_name = NULL;
	auth_sock_dir = NULL;
	return 0;
}
