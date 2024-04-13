inbound_cap_ack (server *serv, char *nick, char *extensions,
					  const message_tags_data *tags_data)
{
	EMIT_SIGNAL_TIMESTAMP (XP_TE_CAPACK, serv->server_session, nick, extensions,
								  NULL, NULL, 0, tags_data->timestamp);

	if (strstr (extensions, "identify-msg") != NULL)
	{
		serv->have_idmsg = TRUE;
	}

	if (strstr (extensions, "multi-prefix") != NULL)
	{
		serv->have_namesx = TRUE;
	}

	if (strstr (extensions, "away-notify") != NULL)
	{
		serv->have_awaynotify = TRUE;
	}

	if (strstr (extensions, "account-notify") != NULL)
	{
		serv->have_accnotify = TRUE;
	}
					
	if (strstr (extensions, "extended-join") != NULL)
	{
		serv->have_extjoin = TRUE;
	}

	if (strstr (extensions, "userhost-in-names") != NULL)
	{
		serv->have_uhnames = TRUE;
	}

	if (strstr (extensions, "server-time") != NULL)
	{
		serv->have_server_time = TRUE;
	}

	if (strstr (extensions, "sasl") != NULL)
	{
		serv->have_sasl = TRUE;
		serv->sent_saslauth = FALSE;

#ifdef USE_OPENSSL
		if (serv->loginmethod == LOGIN_SASLEXTERNAL)
		{
			serv->sasl_mech = MECH_EXTERNAL;
			tcp_send_len (serv, "AUTHENTICATE EXTERNAL\r\n", 23);
		}
		else
		{
			/* default to most secure, it will fallback if not supported */
			serv->sasl_mech = MECH_AES;
			tcp_send_len (serv, "AUTHENTICATE DH-AES\r\n", 21);
		}
#else
		serv->sasl_mech = MECH_PLAIN;
		tcp_send_len (serv, "AUTHENTICATE PLAIN\r\n", 20);
#endif
	}
}