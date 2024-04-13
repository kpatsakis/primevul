ssh_packet_is_interactive(struct ssh *ssh)
{
	return ssh->state->interactive_mode;
}
