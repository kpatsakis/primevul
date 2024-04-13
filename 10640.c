inbound_privmsg (server *serv, char *from, char *ip, char *text, int id,
					  const message_tags_data *tags_data)
{
	session *sess;
	struct User *user;
	char idtext[64];
	gboolean nodiag = FALSE;

	sess = find_dialog (serv, from);

	if (sess || prefs.hex_gui_autoopen_dialog)
	{
		/*0=ctcp  1=priv will set hex_gui_autoopen_dialog=0 here is flud detected */
		if (!sess)
		{
			if (flood_check (from, ip, serv, current_sess, 1))
				/* Create a dialog session */
				sess = inbound_open_dialog (serv, from, tags_data);
			else
				sess = serv->server_session;
			if (!sess)
				return; /* ?? */
		}

		if (ip && ip[0])
			set_topic (sess, ip, ip);
		inbound_chanmsg (serv, NULL, NULL, from, text, FALSE, id, tags_data);
		return;
	}

	sess = find_session_from_nick (from, serv);
	if (!sess)
	{
		sess = serv->front_session;
		nodiag = TRUE; /* We don't want it to look like a normal message in front sess */
	}

	user = userlist_find (sess, from);
	if (user)
	{
		user->lasttalk = time (0);
		if (user->account)
			id = TRUE;
	}
	
	inbound_make_idtext (serv, idtext, sizeof (idtext), id);

	if (sess->type == SESS_DIALOG && !nodiag)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_DPRIVMSG, sess, from, text, idtext, NULL, 0,
									  tags_data->timestamp);
	else
		EMIT_SIGNAL_TIMESTAMP (XP_TE_PRIVMSG, sess, from, text, idtext, NULL, 0, 
									  tags_data->timestamp);
}