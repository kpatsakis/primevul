static void stroke_config(private_stroke_socket_t *this,
						  stroke_msg_t *msg, FILE *out)
{
	this->cred->cachecrl(this->cred, msg->config.cachecrl);
}
