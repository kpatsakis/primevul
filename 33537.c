static void stroke_counters(private_stroke_socket_t *this,
							  stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->counters.name);

	if (msg->counters.reset)
	{
		this->counter->reset(this->counter, msg->counters.name);
	}
	else
	{
		this->counter->print(this->counter, out, msg->counters.name);
	}
}
