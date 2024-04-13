ssh_packet_stop_discard(struct ssh *ssh)
{
	struct session_state *state = ssh->state;
	int r;

	if (state->packet_discard_mac) {
		char buf[1024];

		memset(buf, 'a', sizeof(buf));
		while (sshbuf_len(state->incoming_packet) <
		    PACKET_MAX_SIZE)
			if ((r = sshbuf_put(state->incoming_packet, buf,
			    sizeof(buf))) != 0)
				return r;
		(void) mac_compute(state->packet_discard_mac,
		    state->p_read.seqnr,
		    sshbuf_ptr(state->incoming_packet), PACKET_MAX_SIZE,
		    NULL, 0);
	}
	logit("Finished discarding for %.200s", ssh_remote_ipaddr(ssh));
	return SSH_ERR_MAC_INVALID;
}
