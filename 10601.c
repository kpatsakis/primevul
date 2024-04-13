set_topic (session *sess, char *topic, char *stripped_topic)
{
	/* The topic of dialogs are the users hostname which is logged is new */
	if (sess->type == SESS_DIALOG && (!sess->topic || strcmp(sess->topic, stripped_topic))
		&& sess->logfd != -1)
	{
		char tbuf[1024];
		g_snprintf (tbuf, sizeof (tbuf), "[%s has address %s]\n", sess->channel, stripped_topic);
		write (sess->logfd, tbuf, strlen (tbuf));
	}

	g_free (sess->topic);
	sess->topic = g_strdup (stripped_topic);
	fe_set_topic (sess, topic, stripped_topic);
}