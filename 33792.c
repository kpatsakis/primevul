ssh_packet_backup_state(struct ssh *ssh,
    struct ssh *backup_state)
{
	struct ssh *tmp;

	close(ssh->state->connection_in);
	ssh->state->connection_in = -1;
	close(ssh->state->connection_out);
	ssh->state->connection_out = -1;
	if (backup_state)
		tmp = backup_state;
	else
		tmp = ssh_alloc_session_state();
	backup_state = ssh;
	ssh = tmp;
}
