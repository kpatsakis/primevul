ssh_packet_restore_state(struct ssh *ssh,
    struct ssh *backup_state)
{
	struct ssh *tmp;
	u_int len;
	int r;

	tmp = backup_state;
	backup_state = ssh;
	ssh = tmp;
	ssh->state->connection_in = backup_state->state->connection_in;
	backup_state->state->connection_in = -1;
	ssh->state->connection_out = backup_state->state->connection_out;
	backup_state->state->connection_out = -1;
	len = sshbuf_len(backup_state->state->input);
	if (len > 0) {
		if ((r = sshbuf_putb(ssh->state->input,
		    backup_state->state->input)) != 0)
			fatal("%s: %s", __func__, ssh_err(r));
		sshbuf_reset(backup_state->state->input);
		add_recv_bytes(len);
	}
}
