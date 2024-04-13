static void stroke_status(private_stroke_socket_t *this,
						  stroke_msg_t *msg, FILE *out, bool all, bool wait)
{
	pop_string(msg, &(msg->status.name));

	this->list->status(this->list, msg, out, all, wait);
}
