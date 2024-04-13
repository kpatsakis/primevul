inbound_account (server *serv, char *nick, char *account,
					  const message_tags_data *tags_data)
{
	session *sess = NULL;
	GSList *list;

	list = sess_list;
	while (list)
	{
		sess = list->data;
		if (sess->server == serv)
			userlist_set_account (sess, nick, account);
		list = list->next;
	}
}