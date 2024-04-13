static void stroke_loglevel(private_stroke_socket_t *this,
							stroke_msg_t *msg, FILE *out)
{
	debug_t group;

	pop_string(msg, &(msg->loglevel.type));
	DBG1(DBG_CFG, "received stroke: loglevel %d for %s",
		 msg->loglevel.level, msg->loglevel.type);

	if (this->prevent_loglevel_changes)
	{
		DBG1(DBG_CFG, "prevented log level change");
		fprintf(out, "command not allowed!\n");
		return;
	}
	if (!enum_from_name(debug_names, msg->loglevel.type, &group))
	{
		fprintf(out, "unknown type '%s'!\n", msg->loglevel.type);
		return;
	}
	charon->set_level(charon, group, msg->loglevel.level);
}
