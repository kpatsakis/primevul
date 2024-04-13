inbound_user_info (session *sess, char *chan, char *user, char *host,
						 char *servname, char *nick, char *realname,
						 char *account, unsigned int away,
						 const message_tags_data *tags_data)
{
	server *serv = sess->server;
	session *who_sess;
	GSList *list;
	char *uhost = NULL;

	if (user && host)
	{
		uhost = g_strdup_printf ("%s@%s", user, host);
	}

	if (chan)
	{
		who_sess = find_channel (serv, chan);
		if (who_sess)
			userlist_add_hostname (who_sess, nick, uhost, realname, servname, account, away);
		else
		{
			if (serv->doing_dns && nick && host)
				do_dns (sess, nick, host, tags_data);
		}
	}
	else
	{
		/* came from WHOIS, not channel specific */
		for (list = sess_list; list; list = list->next)
		{
			sess = list->data;
			if (sess->type == SESS_CHANNEL && sess->server == serv)
			{
				userlist_add_hostname (sess, nick, uhost, realname, servname, account, away);
			}
		}
	}

	g_free (uhost);
}