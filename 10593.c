inbound_sasl_supportedmechs (server *serv, char *list)
{
	int i;

	if (serv->sasl_mech != MECH_EXTERNAL)
	{
		/* Use most secure one supported */
		for (i = MECH_AES; i >= MECH_PLAIN; i--)
		{
			if (strstr (list, sasl_mechanisms[i]) != NULL)
			{
				serv->sasl_mech = i;
				serv->retry_sasl = TRUE;
				tcp_sendf (serv, "AUTHENTICATE %s\r\n", sasl_mechanisms[i]);
				return;
			}
		}
	}

	/* Abort, none supported */
	serv->sent_saslauth = TRUE;
	tcp_sendf (serv, "AUTHENTICATE *\r\n");
	return;
}