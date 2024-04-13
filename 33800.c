ssh_packet_get_connection_out(struct ssh *ssh)
{
	return ssh->state->connection_out;
}
