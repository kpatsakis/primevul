inbound_away (server *serv, char *nick, char *msg,
				  const message_tags_data *tags_data)
{
	struct away_msg *away = server_away_find_message (serv, nick);
	session *sess = NULL;
	GSList *list;

	if (away && !strcmp (msg, away->message))	/* Seen the msg before? */
	{
		if (prefs.hex_away_show_once && !serv->inside_whois)
			return;
	} else
	{
		server_away_save_message (serv, nick, msg);
	}

	if (prefs.hex_irc_whois_front)
		sess = serv->front_session;
	else
	{
		if (!serv->inside_whois)
			sess = find_session_from_nick (nick, serv);
		if (!sess)
			sess = serv->server_session;
	}

	/* possibly hide the output */
	if (!serv->inside_whois || !serv->skip_next_whois)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_WHOIS5, sess, nick, msg, NULL, NULL, 0,
									  tags_data->timestamp);

	list = sess_list;
	while (list)
	{
		sess = list->data;
		if (sess->server == serv)
			userlist_set_away (sess, nick, TRUE);
		list = list->next;
	}
}