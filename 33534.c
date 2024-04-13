static void stroke_add_ca(private_stroke_socket_t *this,
						  stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->add_ca.name);
	DBG1(DBG_CFG, "received stroke: add ca '%s'", msg->add_ca.name);

	pop_string(msg, &msg->add_ca.cacert);
	pop_string(msg, &msg->add_ca.crluri);
	pop_string(msg, &msg->add_ca.crluri2);
	pop_string(msg, &msg->add_ca.ocspuri);
	pop_string(msg, &msg->add_ca.ocspuri2);
	pop_string(msg, &msg->add_ca.certuribase);
	DBG2(DBG_CFG, "ca %s", msg->add_ca.name);
	DBG_OPT("  cacert=%s", msg->add_ca.cacert);
	DBG_OPT("  crluri=%s", msg->add_ca.crluri);
	DBG_OPT("  crluri2=%s", msg->add_ca.crluri2);
	DBG_OPT("  ocspuri=%s", msg->add_ca.ocspuri);
	DBG_OPT("  ocspuri2=%s", msg->add_ca.ocspuri2);
	DBG_OPT("  certuribase=%s", msg->add_ca.certuribase);

	this->ca->add(this->ca, msg);
}
