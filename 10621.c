is_hilight (char *from, char *text, session *sess, server *serv)
{
	if (alert_match_word (from, prefs.hex_irc_no_hilight))
		return 0;

	text = strip_color (text, -1, STRIP_ALL);

	if (alert_match_text (text, serv->nick) ||
		 alert_match_text (text, prefs.hex_irc_extra_hilight) ||
		 alert_match_word (from, prefs.hex_irc_nick_hilight))
	{
		g_free (text);
		if (sess != current_tab)
		{
			sess->nick_said = TRUE;
			lastact_update (sess);
		}
		fe_set_hilight (sess);
		return 1;
	}

	g_free (text);
	return 0;
}