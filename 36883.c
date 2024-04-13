cifs_find_tcp_session(struct sockaddr *addr, struct smb_vol *vol)
{
	struct TCP_Server_Info *server;

	spin_lock(&cifs_tcp_ses_lock);
	list_for_each_entry(server, &cifs_tcp_ses_list, tcp_ses_list) {
		if (!net_eq(cifs_net_ns(server), current->nsproxy->net_ns))
			continue;

		if (!match_address(server, addr,
				   (struct sockaddr *)&vol->srcaddr))
			continue;

		if (!match_port(server, addr))
			continue;

		if (!match_security(server, vol))
			continue;

		++server->srv_count;
		spin_unlock(&cifs_tcp_ses_lock);
		cFYI(1, "Existing tcp session with server found");
		return server;
	}
	spin_unlock(&cifs_tcp_ses_lock);
	return NULL;
}
