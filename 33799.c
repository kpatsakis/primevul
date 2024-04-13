ssh_packet_get_connection_in(struct ssh *ssh)
{
	return ssh->state->connection_in;
}
