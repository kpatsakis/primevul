static void stroke_del_conn(private_stroke_socket_t *this, stroke_msg_t *msg)
{
	pop_string(msg, &msg->del_conn.name);
	DBG1(DBG_CFG, "received stroke: delete connection '%s'", msg->del_conn.name);

	this->config->del(this->config, msg);
	this->attribute->del_dns(this->attribute, msg);
	this->handler->del_attributes(this->handler, msg);
}
