ssh_packet_get_maxsize(struct ssh *ssh)
{
	return ssh->state->max_packet_size;
}
