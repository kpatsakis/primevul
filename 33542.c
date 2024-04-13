static void stroke_leases(private_stroke_socket_t *this,
						  stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->leases.pool);
	pop_string(msg, &msg->leases.address);

	this->list->leases(this->list, msg, out);
}
