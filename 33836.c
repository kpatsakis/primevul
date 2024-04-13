ssh_remote_ipaddr(struct ssh *ssh)
{
	/* Check whether we have cached the ipaddr. */
	if (ssh->remote_ipaddr == NULL)
		ssh->remote_ipaddr = ssh_packet_connection_is_on_socket(ssh) ?
		    get_peer_ipaddr(ssh->state->connection_in) :
		    strdup("UNKNOWN");
	if (ssh->remote_ipaddr == NULL)
		return "UNKNOWN";
	return ssh->remote_ipaddr;
}
