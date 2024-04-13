inbound_login_start (session *sess, char *nick, char *servname,
							const message_tags_data *tags_data)
{
	inbound_newnick (sess->server, sess->server->nick, nick, TRUE, tags_data);
	server_set_name (sess->server, servname);
	if (sess->type == SESS_SERVER)
		log_open_or_close (sess);
	/* reset our away status */
	if (sess->server->reconnect_away)
	{
		handle_command (sess->server->server_session, "away", FALSE);
		sess->server->reconnect_away = FALSE;
	}
}