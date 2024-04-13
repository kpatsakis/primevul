inbound_next_nick (session *sess, char *nick, int error,
						 const message_tags_data *tags_data)
{
	char *newnick;
	server *serv = sess->server;
	ircnet *net;

	serv->nickcount++;

	switch (serv->nickcount)
	{
	case 2:
		newnick = prefs.hex_irc_nick2;
		net = serv->network;
		/* use network specific "Second choice"? */
		if (net && !(net->flags & FLAG_USE_GLOBAL) && net->nick2)
		{
			newnick = net->nick2;
		}
		serv->p_change_nick (serv, newnick);
		if (error)
		{
			EMIT_SIGNAL_TIMESTAMP (XP_TE_NICKERROR, sess, nick, newnick, NULL, NULL,
										  0, tags_data->timestamp);
		}
		else
		{
			EMIT_SIGNAL_TIMESTAMP (XP_TE_NICKCLASH, sess, nick, newnick, NULL, NULL,
										  0, tags_data->timestamp);
		}
		break;

	case 3:
		serv->p_change_nick (serv, prefs.hex_irc_nick3);
		if (error)
		{
			EMIT_SIGNAL_TIMESTAMP (XP_TE_NICKERROR, sess, nick, prefs.hex_irc_nick3,
										  NULL, NULL, 0, tags_data->timestamp);
		}
		else
		{
			EMIT_SIGNAL_TIMESTAMP (XP_TE_NICKCLASH, sess, nick, prefs.hex_irc_nick3,
										  NULL, NULL, 0, tags_data->timestamp);
		}
		break;

	default:
		EMIT_SIGNAL_TIMESTAMP (XP_TE_NICKFAIL, sess, NULL, NULL, NULL, NULL, 0,
									  tags_data->timestamp);
	}
}