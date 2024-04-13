inbound_topic (server *serv, char *chan, char *topic_text,
					const message_tags_data *tags_data)
{
	session *sess = find_channel (serv, chan);
	char *stripped_topic;

	if (sess)
	{
		stripped_topic = strip_color (topic_text, -1, STRIP_ALL);
		set_topic (sess, topic_text, stripped_topic);
		g_free (stripped_topic);
	} else
		sess = serv->server_session;

	EMIT_SIGNAL_TIMESTAMP (XP_TE_TOPIC, sess, chan, topic_text, NULL, NULL, 0,
								  tags_data->timestamp);
}