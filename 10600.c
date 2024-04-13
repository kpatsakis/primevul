inbound_away_notify (server *serv, char *nick, char *reason,
							const message_tags_data *tags_data)
{
	session *sess = NULL;
	GSList *list;

	list = sess_list;
	while (list)
	{
		sess = list->data;
		if (sess->server == serv)
		{
			userlist_set_away (sess, nick, reason ? TRUE : FALSE);
			if (sess == serv->front_session && notify_is_in_list (serv, nick))
			{
				if (reason)
					EMIT_SIGNAL_TIMESTAMP (XP_TE_NOTIFYAWAY, sess, nick, reason, NULL,
												  NULL, 0, tags_data->timestamp);
				else
					EMIT_SIGNAL_TIMESTAMP (XP_TE_NOTIFYBACK, sess, nick, NULL, NULL, 
												  NULL, 0, tags_data->timestamp);
			}
		}
		list = list->next;
	}
}