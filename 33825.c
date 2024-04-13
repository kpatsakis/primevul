ssh_packet_set_encryption_key(struct ssh *ssh, const u_char *key, u_int keylen, int number)
{
#ifndef WITH_SSH1
	fatal("no SSH protocol 1 support");
#else /* WITH_SSH1 */
	struct session_state *state = ssh->state;
	const struct sshcipher *cipher = cipher_by_number(number);
	int r;
	const char *wmsg;

	if (cipher == NULL)
		fatal("%s: unknown cipher number %d", __func__, number);
	if (keylen < 20)
		fatal("%s: keylen too small: %d", __func__, keylen);
	if (keylen > SSH_SESSION_KEY_LENGTH)
		fatal("%s: keylen too big: %d", __func__, keylen);
	memcpy(state->ssh1_key, key, keylen);
	state->ssh1_keylen = keylen;
	if ((r = cipher_init(&state->send_context, cipher, key, keylen,
	    NULL, 0, CIPHER_ENCRYPT)) != 0 ||
	    (r = cipher_init(&state->receive_context, cipher, key, keylen,
	    NULL, 0, CIPHER_DECRYPT) != 0))
		fatal("%s: cipher_init failed: %s", __func__, ssh_err(r));
	if (!state->cipher_warning_done &&
	    ((wmsg = cipher_warning_message(&state->send_context)) != NULL ||
	    (wmsg = cipher_warning_message(&state->send_context)) != NULL)) {
		error("Warning: %s", wmsg);
		state->cipher_warning_done = 1;
	}
#endif /* WITH_SSH1 */
}
