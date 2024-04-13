sshpkt_send(struct ssh *ssh)
{
	if (compat20)
		return ssh_packet_send2(ssh);
	else
		return ssh_packet_send1(ssh);
}
