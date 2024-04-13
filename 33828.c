ssh_packet_set_server(struct ssh *ssh)
{
	ssh->state->server_side = 1;
}
