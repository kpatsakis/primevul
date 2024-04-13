inbound_chanmsg (server *serv, session *sess, char *chan, char *from, 
					  char *text, char fromme, int id, 
					  const message_tags_data *tags_data)
{
	struct User *user;
	int hilight = FALSE;
	char nickchar[2] = "\000";
	char idtext[64];

	if (!sess)
	{
		if (chan)
		{
			sess = find_channel (serv, chan);
			if (!sess && !is_channel (serv, chan))
				sess = find_dialog (serv, chan);
		} else
		{
			sess = find_dialog (serv, from);
		}
		if (!sess)
			return;
	}

	if (sess != current_tab)
	{
		sess->msg_said = TRUE;
		sess->new_data = FALSE;
		lastact_update (sess);
	}

	user = userlist_find (sess, from);
	if (user)
	{
		if (user->account)
			id = TRUE;
		nickchar[0] = user->prefix[0];
		user->lasttalk = time (0);
		if (user->me)
			fromme = TRUE;
	}

	if (fromme)
	{
		if (prefs.hex_away_auto_unmark && serv->is_away && !tags_data->timestamp)
			sess->server->p_set_back (sess->server);
		EMIT_SIGNAL_TIMESTAMP (XP_TE_UCHANMSG, sess, from, text, nickchar, NULL,
									  0, tags_data->timestamp);
		return;
	}

	inbound_make_idtext (serv, idtext, sizeof (idtext), id);

	if (is_hilight (from, text, sess, serv))
		hilight = TRUE;

	if (sess->type == SESS_DIALOG)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_DPRIVMSG, sess, from, text, idtext, NULL, 0,
									  tags_data->timestamp);
	else if (hilight)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_HCHANMSG, sess, from, text, nickchar, idtext,
									  0, tags_data->timestamp);
	else
		EMIT_SIGNAL_TIMESTAMP (XP_TE_CHANMSG, sess, from, text, nickchar, idtext,
									  0, tags_data->timestamp);
}