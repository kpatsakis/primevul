inbound_topictime (server *serv, char *chan, char *nick, time_t stamp,
						 const message_tags_data *tags_data)
{
	char *tim = ctime (&stamp);
	session *sess = find_channel (serv, chan);

	if (!sess)
		sess = serv->server_session;

	tim[24] = 0;	/* get rid of the \n */
	EMIT_SIGNAL_TIMESTAMP (XP_TE_TOPICDATE, sess, chan, nick, tim, NULL, 0,
								  tags_data->timestamp);
}