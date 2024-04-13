static void stroke_terminate_srcip(private_stroke_socket_t *this,
								   stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->terminate_srcip.start);
	pop_string(msg, &msg->terminate_srcip.end);
	DBG1(DBG_CFG, "received stroke: terminate-srcip %s-%s",
		 msg->terminate_srcip.start, msg->terminate_srcip.end);

	this->control->terminate_srcip(this->control, msg, out);
}
