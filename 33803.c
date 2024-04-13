ssh_packet_get_output(struct ssh *ssh)
{
	return (void *)ssh->state->output;
}
