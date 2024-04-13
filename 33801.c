ssh_packet_get_input(struct ssh *ssh)
{
	return (void *)ssh->state->input;
}
