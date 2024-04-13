static void stroke_unroute(private_stroke_socket_t *this, stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->terminate.name);
	DBG1(DBG_CFG, "received stroke: unroute '%s'", msg->route.name);

	this->control->unroute(this->control, msg, out);
}
