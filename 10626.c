inbound_cap_list (server *serv, char *nick, char *extensions,
						const message_tags_data *tags_data)
{
	EMIT_SIGNAL_TIMESTAMP (XP_TE_CAPACK, serv->server_session, nick, extensions,
								  NULL, NULL, 0, tags_data->timestamp);
}