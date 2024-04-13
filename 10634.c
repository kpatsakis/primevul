inbound_quit (server *serv, char *nick, char *ip, char *reason,
				  const message_tags_data *tags_data)
{
	GSList *list = sess_list;
	session *sess;
	struct User *user;
	int was_on_front_session = FALSE;

	while (list)
	{
		sess = (session *) list->data;
		if (sess->server == serv)
		{
 			if (sess == current_sess)
 				was_on_front_session = TRUE;
			if ((user = userlist_find (sess, nick)))
			{
				EMIT_SIGNAL_TIMESTAMP (XP_TE_QUIT, sess, nick, reason, ip, NULL, 0,
											  tags_data->timestamp);
				userlist_remove_user (sess, user);
			} else if (sess->type == SESS_DIALOG && !serv->p_cmp (sess->channel, nick))
			{
				EMIT_SIGNAL_TIMESTAMP (XP_TE_QUIT, sess, nick, reason, ip, NULL, 0,
											  tags_data->timestamp);
			}
		}
		list = list->next;
	}

	notify_set_offline (serv, nick, was_on_front_session, tags_data);
}