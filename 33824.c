ssh_packet_set_connection(struct ssh *ssh, int fd_in, int fd_out)
{
	struct session_state *state;
	const struct sshcipher *none = cipher_by_name("none");
	int r;

	if (none == NULL) {
		error("%s: cannot load cipher 'none'", __func__);
		return NULL;
	}
	if (ssh == NULL)
		ssh = ssh_alloc_session_state();
	if (ssh == NULL) {
		error("%s: cound not allocate state", __func__);
		return NULL;
	}
	state = ssh->state;
	state->connection_in = fd_in;
	state->connection_out = fd_out;
	if ((r = cipher_init(&state->send_context, none,
	    (const u_char *)"", 0, NULL, 0, CIPHER_ENCRYPT)) != 0 ||
	    (r = cipher_init(&state->receive_context, none,
	    (const u_char *)"", 0, NULL, 0, CIPHER_DECRYPT)) != 0) {
		error("%s: cipher_init failed: %s", __func__, ssh_err(r));
		free(ssh);
		return NULL;
	}
	state->newkeys[MODE_IN] = state->newkeys[MODE_OUT] = NULL;
	deattack_init(&state->deattack);
	/*
	 * Cache the IP address of the remote connection for use in error
	 * messages that might be generated after the connection has closed.
	 */
	(void)ssh_remote_ipaddr(ssh);
	return ssh;
}
