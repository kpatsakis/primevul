inbound_topicnew (server *serv, char *nick, char *chan, char *topic,
						const message_tags_data *tags_data)
{
	session *sess;
	char *stripped_topic;

	sess = find_channel (serv, chan);
	if (sess)
	{
		EMIT_SIGNAL_TIMESTAMP (XP_TE_NEWTOPIC, sess, nick, topic, chan, NULL, 0,
									  tags_data->timestamp);
		stripped_topic = strip_color (topic, -1, STRIP_ALL);
		set_topic (sess, topic, stripped_topic);
		g_free (stripped_topic);
	}
}