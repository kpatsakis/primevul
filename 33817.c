ssh_packet_send1(struct ssh *ssh)
{
	struct session_state *state = ssh->state;
	u_char buf[8], *cp;
	int r, padding, len;
	u_int checksum;

	/*
	 * If using packet compression, compress the payload of the outgoing
	 * packet.
	 */
	if (state->packet_compression) {
		sshbuf_reset(state->compression_buffer);
		/* Skip padding. */
		if ((r = sshbuf_consume(state->outgoing_packet, 8)) != 0)
			goto out;
		/* padding */
		if ((r = sshbuf_put(state->compression_buffer,
		    "\0\0\0\0\0\0\0\0", 8)) != 0)
			goto out;
		if ((r = compress_buffer(ssh, state->outgoing_packet,
		    state->compression_buffer)) != 0)
			goto out;
		sshbuf_reset(state->outgoing_packet);
                if ((r = sshbuf_putb(state->outgoing_packet,
                    state->compression_buffer)) != 0)
			goto out;
	}
	/* Compute packet length without padding (add checksum, remove padding). */
	len = sshbuf_len(state->outgoing_packet) + 4 - 8;

	/* Insert padding. Initialized to zero in packet_start1() */
	padding = 8 - len % 8;
	if (!state->send_context.plaintext) {
		cp = sshbuf_mutable_ptr(state->outgoing_packet);
		if (cp == NULL) {
			r = SSH_ERR_INTERNAL_ERROR;
			goto out;
		}
		arc4random_buf(cp + 8 - padding, padding);
	}
	if ((r = sshbuf_consume(state->outgoing_packet, 8 - padding)) != 0)
		goto out;

	/* Add check bytes. */
	checksum = ssh_crc32(sshbuf_ptr(state->outgoing_packet),
	    sshbuf_len(state->outgoing_packet));
	POKE_U32(buf, checksum);
	if ((r = sshbuf_put(state->outgoing_packet, buf, 4)) != 0)
		goto out;

#ifdef PACKET_DEBUG
	fprintf(stderr, "packet_send plain: ");
	sshbuf_dump(state->outgoing_packet, stderr);
#endif

	/* Append to output. */
	POKE_U32(buf, len);
	if ((r = sshbuf_put(state->output, buf, 4)) != 0)
		goto out;
	if ((r = sshbuf_reserve(state->output,
	    sshbuf_len(state->outgoing_packet), &cp)) != 0)
		goto out;
	if ((r = cipher_crypt(&state->send_context, 0, cp,
	    sshbuf_ptr(state->outgoing_packet),
	    sshbuf_len(state->outgoing_packet), 0, 0)) != 0)
		goto out;

#ifdef PACKET_DEBUG
	fprintf(stderr, "encrypted: ");
	sshbuf_dump(state->output, stderr);
#endif
	state->p_send.packets++;
	state->p_send.bytes += len +
	    sshbuf_len(state->outgoing_packet);
	sshbuf_reset(state->outgoing_packet);

	/*
	 * Note that the packet is now only buffered in output.  It won't be
	 * actually sent until ssh_packet_write_wait or ssh_packet_write_poll
	 * is called.
	 */
	r = 0;
 out:
	return r;
}
