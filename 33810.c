ssh_packet_need_rekeying(struct ssh *ssh)
{
	struct session_state *state = ssh->state;

	if (ssh->compat & SSH_BUG_NOREKEY)
		return 0;
	return
	    (state->p_send.packets > MAX_PACKETS) ||
	    (state->p_read.packets > MAX_PACKETS) ||
	    (state->max_blocks_out &&
	        (state->p_send.blocks > state->max_blocks_out)) ||
	    (state->max_blocks_in &&
	        (state->p_read.blocks > state->max_blocks_in)) ||
	    (state->rekey_interval != 0 && state->rekey_time +
		 state->rekey_interval <= monotime());
}
