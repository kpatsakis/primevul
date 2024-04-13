inbound_open_dialog (server *serv, char *from,
							const message_tags_data *tags_data)
{
	session *sess;

	sess = new_ircwindow (serv, from, SESS_DIALOG, 0);
	/* for playing sounds */
	EMIT_SIGNAL_TIMESTAMP (XP_TE_OPENDIALOG, sess, NULL, NULL, NULL, NULL, 0,
								  tags_data->timestamp);

	return sess;
}