ssh_packet_read_poll1(struct ssh *ssh, u_char *typep)
{
	struct session_state *state = ssh->state;
	u_int len, padded_len;
	const char *emsg;
	const u_char *cp;
	u_char *p;
	u_int checksum, stored_checksum;
	int r;

	*typep = SSH_MSG_NONE;

	/* Check if input size is less than minimum packet size. */
	if (sshbuf_len(state->input) < 4 + 8)
		return 0;
	/* Get length of incoming packet. */
	len = PEEK_U32(sshbuf_ptr(state->input));
	if (len < 1 + 2 + 2 || len > 256 * 1024) {
		if ((r = sshpkt_disconnect(ssh, "Bad packet length %u",
		    len)) != 0)
			return r;
		return SSH_ERR_CONN_CORRUPT;
	}
	padded_len = (len + 8) & ~7;

	/* Check if the packet has been entirely received. */
	if (sshbuf_len(state->input) < 4 + padded_len)
		return 0;

	/* The entire packet is in buffer. */

	/* Consume packet length. */
	if ((r = sshbuf_consume(state->input, 4)) != 0)
		goto out;

	/*
	 * Cryptographic attack detector for ssh
	 * (C)1998 CORE-SDI, Buenos Aires Argentina
	 * Ariel Futoransky(futo@core-sdi.com)
	 */
	if (!state->receive_context.plaintext) {
		emsg = NULL;
		switch (detect_attack(&state->deattack,
		    sshbuf_ptr(state->input), padded_len)) {
		case DEATTACK_OK:
			break;
		case DEATTACK_DETECTED:
			emsg = "crc32 compensation attack detected";
			break;
		case DEATTACK_DOS_DETECTED:
			emsg = "deattack denial of service detected";
			break;
		default:
			emsg = "deattack error";
			break;
		}
		if (emsg != NULL) {
			error("%s", emsg);
			if ((r = sshpkt_disconnect(ssh, "%s", emsg)) != 0 ||
			    (r = ssh_packet_write_wait(ssh)) != 0)
					return r;
			return SSH_ERR_CONN_CORRUPT;
		}
	}

	/* Decrypt data to incoming_packet. */
	sshbuf_reset(state->incoming_packet);
	if ((r = sshbuf_reserve(state->incoming_packet, padded_len, &p)) != 0)
		goto out;
	if ((r = cipher_crypt(&state->receive_context, 0, p,
	    sshbuf_ptr(state->input), padded_len, 0, 0)) != 0)
		goto out;

	if ((r = sshbuf_consume(state->input, padded_len)) != 0)
		goto out;

#ifdef PACKET_DEBUG
	fprintf(stderr, "read_poll plain: ");
	sshbuf_dump(state->incoming_packet, stderr);
#endif

	/* Compute packet checksum. */
	checksum = ssh_crc32(sshbuf_ptr(state->incoming_packet),
	    sshbuf_len(state->incoming_packet) - 4);

	/* Skip padding. */
	if ((r = sshbuf_consume(state->incoming_packet, 8 - len % 8)) != 0)
		goto out;

	/* Test check bytes. */
	if (len != sshbuf_len(state->incoming_packet)) {
		error("%s: len %d != sshbuf_len %zd", __func__,
		    len, sshbuf_len(state->incoming_packet));
		if ((r = sshpkt_disconnect(ssh, "invalid packet length")) != 0 ||
		    (r = ssh_packet_write_wait(ssh)) != 0)
			return r;
		return SSH_ERR_CONN_CORRUPT;
	}

	cp = sshbuf_ptr(state->incoming_packet) + len - 4;
	stored_checksum = PEEK_U32(cp);
	if (checksum != stored_checksum) {
		error("Corrupted check bytes on input");
		if ((r = sshpkt_disconnect(ssh, "connection corrupted")) != 0 ||
		    (r = ssh_packet_write_wait(ssh)) != 0)
			return r;
		return SSH_ERR_CONN_CORRUPT;
	}
	if ((r = sshbuf_consume_end(state->incoming_packet, 4)) < 0)
		goto out;

	if (state->packet_compression) {
		sshbuf_reset(state->compression_buffer);
		if ((r = uncompress_buffer(ssh, state->incoming_packet,
		    state->compression_buffer)) != 0)
			goto out;
		sshbuf_reset(state->incoming_packet);
		if ((r = sshbuf_putb(state->incoming_packet,
		    state->compression_buffer)) != 0)
			goto out;
	}
	state->p_read.packets++;
	state->p_read.bytes += padded_len + 4;
	if ((r = sshbuf_get_u8(state->incoming_packet, typep)) != 0)
		goto out;
	if (*typep < SSH_MSG_MIN || *typep > SSH_MSG_MAX) {
		error("Invalid ssh1 packet type: %d", *typep);
		if ((r = sshpkt_disconnect(ssh, "invalid packet type")) != 0 ||
		    (r = ssh_packet_write_wait(ssh)) != 0)
			return r;
		return SSH_ERR_PROTOCOL_ERROR;
	}
	r = 0;
 out:
	return r;
}
