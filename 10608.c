inbound_sasl_authenticate (server *serv, char *data)
{
		ircnet *net = (ircnet*)serv->network;
		char *user, *pass = NULL;
		const char *mech = sasl_mechanisms[serv->sasl_mech];

		/* Got a list of supported mechanisms from inspircd */
		if (strchr (data, ',') != NULL)
		{
			inbound_sasl_supportedmechs (serv, data);
			return;
		}

		if (net->user && !(net->flags & FLAG_USE_GLOBAL))
			user = net->user;
		else
			user = prefs.hex_irc_user_name;

		switch (serv->sasl_mech)
		{
		case MECH_PLAIN:
			pass = encode_sasl_pass_plain (user, serv->password);
			break;
#ifdef USE_OPENSSL
		case MECH_BLOWFISH:
			pass = encode_sasl_pass_blowfish (user, serv->password, data);
			break;
		case MECH_AES:
			pass = encode_sasl_pass_aes (user, serv->password, data);
			break;
		case MECH_EXTERNAL:
			pass = g_strdup ("+");
			break;
#endif
		}

		if (pass == NULL)
		{
			/* something went wrong abort */
			serv->sent_saslauth = TRUE; /* prevent trying PLAIN */
			tcp_sendf (serv, "AUTHENTICATE *\r\n");
			return;
		}

		serv->sent_saslauth = TRUE;
		tcp_sendf (serv, "AUTHENTICATE %s\r\n", pass);
		g_free (pass);

		
		EMIT_SIGNAL_TIMESTAMP (XP_TE_SASLAUTH, serv->server_session, user, (char*)mech,
								NULL,	NULL,	0,	0);
}