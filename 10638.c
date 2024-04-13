inbound_foundip (session *sess, char *ip, const message_tags_data *tags_data)
{
	struct hostent *HostAddr;

	HostAddr = gethostbyname (ip);
	if (HostAddr)
	{
		prefs.dcc_ip = ((struct in_addr *) HostAddr->h_addr)->s_addr;
		EMIT_SIGNAL_TIMESTAMP (XP_TE_FOUNDIP, sess->server->server_session,
									  inet_ntoa (*((struct in_addr *) HostAddr->h_addr)),
									  NULL, NULL, NULL, 0, tags_data->timestamp);
	}
}