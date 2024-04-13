inbound_nameslist (server *serv, char *chan, char *names,
						 const message_tags_data *tags_data)
{
	session *sess;
	char **name_list;
	char *host, *nopre_name;
	char name[NICKLEN];
	int i;
	size_t offset;

	sess = find_channel (serv, chan);
	if (!sess)
	{
		EMIT_SIGNAL_TIMESTAMP (XP_TE_USERSONCHAN, serv->server_session, chan,
									  names, NULL, NULL, 0, tags_data->timestamp);
		return;
	}
	if (!sess->ignore_names)
		EMIT_SIGNAL_TIMESTAMP (XP_TE_USERSONCHAN, sess, chan, names, NULL, NULL,
									  0, tags_data->timestamp);

	if (sess->end_of_names)
	{
		sess->end_of_names = FALSE;
		userlist_clear (sess);
	}

	name_list = g_strsplit (names, " ", -1);
	for (i = 0; name_list[i]; i++)
	{
		host = NULL;
		offset = sizeof(name);

		if (name_list[i][0] == 0)
			continue;

		if (serv->have_uhnames)
		{
			offset = 0;
			nopre_name = name_list[i];

			/* Ignore prefixes so '!' won't cause issues */
			while (strchr (serv->nick_prefixes, *nopre_name) != NULL)
			{
				nopre_name++;
				offset++;
			}

			offset += strcspn (nopre_name, "!");
			if (offset++ < strlen (name_list[i]))
				host = name_list[i] + offset;
		}

		g_strlcpy (name, name_list[i], MIN(offset, sizeof(name)));

		userlist_add (sess, name, host, NULL, NULL, tags_data);
	}
	g_strfreev (name_list);
}