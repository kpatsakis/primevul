ssh_packet_close(struct ssh *ssh)
{
	struct session_state *state = ssh->state;
	int r;
	u_int mode;

	if (!state->initialized)
		return;
	state->initialized = 0;
	if (state->connection_in == state->connection_out) {
		shutdown(state->connection_out, SHUT_RDWR);
		close(state->connection_out);
	} else {
		close(state->connection_in);
		close(state->connection_out);
	}
	sshbuf_free(state->input);
	sshbuf_free(state->output);
	sshbuf_free(state->outgoing_packet);
	sshbuf_free(state->incoming_packet);
	for (mode = 0; mode < MODE_MAX; mode++)
		kex_free_newkeys(state->newkeys[mode]);
	if (state->compression_buffer) {
		sshbuf_free(state->compression_buffer);
		if (state->compression_out_started) {
			z_streamp stream = &state->compression_out_stream;
			debug("compress outgoing: "
			    "raw data %llu, compressed %llu, factor %.2f",
				(unsigned long long)stream->total_in,
				(unsigned long long)stream->total_out,
				stream->total_in == 0 ? 0.0 :
				(double) stream->total_out / stream->total_in);
			if (state->compression_out_failures == 0)
				deflateEnd(stream);
		}
		if (state->compression_in_started) {
			z_streamp stream = &state->compression_out_stream;
			debug("compress incoming: "
			    "raw data %llu, compressed %llu, factor %.2f",
			    (unsigned long long)stream->total_out,
			    (unsigned long long)stream->total_in,
			    stream->total_out == 0 ? 0.0 :
			    (double) stream->total_in / stream->total_out);
			if (state->compression_in_failures == 0)
				inflateEnd(stream);
		}
	}
	if ((r = cipher_cleanup(&state->send_context)) != 0)
		error("%s: cipher_cleanup failed: %s", __func__, ssh_err(r));
	if ((r = cipher_cleanup(&state->receive_context)) != 0)
		error("%s: cipher_cleanup failed: %s", __func__, ssh_err(r));
	if (ssh->remote_ipaddr) {
		free(ssh->remote_ipaddr);
		ssh->remote_ipaddr = NULL;
	}
	free(ssh->state);
	ssh->state = NULL;
}
