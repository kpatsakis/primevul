check_autojoin_channels (server *serv)
{
	int i = 0;
	session *sess;
	GSList *list = sess_list;
	GSList *sess_channels = NULL;			/* joined channels that are not in the favorites list */
	favchannel *fav;

	/* shouldn't really happen, the io tag is destroyed in server.c */
	if (!is_server (serv))
	{
		return FALSE;
	}

	/* If there's a session (i.e. this is a reconnect), autojoin to everything that was open previously. */
	while (list)
	{
		sess = list->data;

		if (sess->server == serv)
		{
			if (sess->willjoinchannel[0] != 0)
			{
				strcpy (sess->waitchannel, sess->willjoinchannel);
				sess->willjoinchannel[0] = 0;

				fav = servlist_favchan_find (serv->network, sess->waitchannel, NULL);	/* Is this channel in our favorites? */

				/* session->channelkey is initially unset for channels joined from the favorites. You have to fill them up manually from favorites settings. */
				if (fav)
				{
					/* session->channelkey is set if there was a key change during the session. In that case, use the session key, not the one from favorites. */
					if (fav->key && !strlen (sess->channelkey))
					{
						safe_strcpy (sess->channelkey, fav->key, sizeof (sess->channelkey));
					}
				}

				/* for easier checks, ensure that favchannel->key is just NULL when session->channelkey is empty i.e. '' */
				if (strlen (sess->channelkey))
				{
					sess_channels = servlist_favchan_listadd (sess_channels, sess->waitchannel, sess->channelkey);
				}
				else
				{
					sess_channels = servlist_favchan_listadd (sess_channels, sess->waitchannel, NULL);
				}
				i++;
			}
		}

		list = list->next;
	}

	if (sess_channels)
	{
		serv->p_join_list (serv, sess_channels);
		g_slist_free_full (sess_channels, (GDestroyNotify) servlist_favchan_free);
	}
	else
	{
		/* If there's no session, just autojoin to favorites. */
		if (serv->favlist)
		{
			serv->p_join_list (serv, serv->favlist);
			i++;

			/* FIXME this is not going to work and is not needed either. server_free() does the job already. */
			/* g_slist_free_full (serv->favlist, (GDestroyNotify) servlist_favchan_free); */
		}
	}

	serv->joindelay_tag = 0;
	fe_server_event (serv, FE_SE_LOGGEDIN, i);
	return FALSE;
}