static void stroke_route(private_stroke_socket_t *this, stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->route.name);
	DBG1(DBG_CFG, "received stroke: route '%s'", msg->route.name);

	this->control->route(this->control, msg, out);
}
