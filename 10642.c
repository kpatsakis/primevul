inbound_login_end (session *sess, char *text, const message_tags_data *tags_data)
{
	GSList *cmdlist;
	commandentry *cmd;
	server *serv = sess->server;

	if (!serv->end_of_motd)
	{
		if (prefs.hex_dcc_ip_from_server && serv->use_who)
		{
			serv->skip_next_userhost = TRUE;
			serv->p_get_ip_uh (serv, serv->nick);	/* sends USERHOST mynick */
		}
		set_default_modes (serv);

		if (serv->network)
		{
			/* there may be more than 1, separated by \n */

			cmdlist = ((ircnet *)serv->network)->commandlist;
			while (cmdlist)
			{
				cmd = cmdlist->data;
				inbound_exec_eom_cmd (cmd->command, sess);
				cmdlist = cmdlist->next;
			}

			/* send nickserv password */
			if (((ircnet *)serv->network)->pass && inbound_nickserv_login (serv))
			{
				serv->p_ns_identify (serv, ((ircnet *)serv->network)->pass);
			}
		}

		/* wait for join if command or nickserv set */
		if (serv->network && prefs.hex_irc_join_delay
			&& ((((ircnet *)serv->network)->pass && inbound_nickserv_login (serv))
				|| ((ircnet *)serv->network)->commandlist))
		{
			serv->joindelay_tag = fe_timeout_add (prefs.hex_irc_join_delay * 1000, check_autojoin_channels, serv);
		}
		else
		{
			check_autojoin_channels (serv);
		}

		if (serv->supports_watch || serv->supports_monitor)
		{
			notify_send_watches (serv);
		}

		serv->end_of_motd = TRUE;
	}

	if (prefs.hex_irc_skip_motd && !serv->motd_skipped)
	{
		serv->motd_skipped = TRUE;
		EMIT_SIGNAL_TIMESTAMP (XP_TE_MOTDSKIP, serv->server_session, NULL, NULL,
									  NULL, NULL, 0, tags_data->timestamp);
		return;
	}

	EMIT_SIGNAL_TIMESTAMP (XP_TE_MOTD, serv->server_session, text, NULL, NULL,
								  NULL, 0, tags_data->timestamp);
}