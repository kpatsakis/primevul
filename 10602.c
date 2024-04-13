inbound_action (session *sess, char *chan, char *from, char *ip, char *text,
					 int fromme, int id, const message_tags_data *tags_data)
{
	session *def = sess;
	server *serv = sess->server;
	struct User *user;
	char nickchar[2] = "\000";
	char idtext[64];
	int privaction = FALSE;

	if (!fromme)
	{
		if (is_channel (serv, chan))
		{
			sess = find_channel (serv, chan);
		} else
		{
			/* it's a private action! */
			privaction = TRUE;
			/* find a dialog tab for it */
			sess = find_dialog (serv, from);
			/* if non found, open a new one */
			if (!sess && prefs.hex_gui_autoopen_dialog)
			{
				/* but only if it wouldn't flood */
				if (flood_check (from, ip, serv, current_sess, 1))
					sess = inbound_open_dialog (serv, from, tags_data);
				else
					sess = serv->server_session;
			}
			if (!sess)
			{
				sess = find_session_from_nick (from, serv);
				/* still not good? */
				if (!sess)
					sess = serv->front_session;
			}
		}
	}

	if (!sess)
		sess = def;

	if (sess != current_tab)
	{
		if (fromme)
		{
			sess->msg_said = FALSE;
			sess->new_data = TRUE;
		} else
		{
			sess->msg_said = TRUE;
			sess->new_data = FALSE;
		}
		lastact_update (sess);
	}

	user = userlist_find (sess, from);
	if (user)
	{
		nickchar[0] = user->prefix[0];
		user->lasttalk = time (0);
		if (user->account)
			id = TRUE;
		if (user->me)
			fromme = TRUE;
	}

	inbound_make_idtext (serv, idtext, sizeof (idtext), id);

	if (!fromme && !privaction)
	{
		if (is_hilight (from, text, sess, serv))
		{
			EMIT_SIGNAL_TIMESTAMP (XP_TE_HCHANACTION, sess, from, text, nickchar,
										  idtext, 0, tags_data->timestamp);
			return;
		}
	}

	if (fromme)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_UACTION, sess, from, text, nickchar, idtext,
									  0, tags_data->timestamp);
	else if (!privaction)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_CHANACTION, sess, from, text, nickchar,
									  idtext, 0, tags_data->timestamp);
	else if (sess->type == SESS_DIALOG)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_DPRIVACTION, sess, from, text, idtext, NULL,
									  0, tags_data->timestamp);
	else
		EMIT_SIGNAL_TIMESTAMP (XP_TE_PRIVACTION, sess, from, text, idtext, NULL, 0,
									  tags_data->timestamp);
}