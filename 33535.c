static void stroke_add_conn(private_stroke_socket_t *this, stroke_msg_t *msg)
{
	pop_string(msg, &msg->add_conn.name);
	DBG1(DBG_CFG, "received stroke: add connection '%s'", msg->add_conn.name);

	DBG2(DBG_CFG, "conn %s", msg->add_conn.name);
	pop_end(msg, "left", &msg->add_conn.me);
	pop_end(msg, "right", &msg->add_conn.other);
	pop_string(msg, &msg->add_conn.eap_identity);
	pop_string(msg, &msg->add_conn.aaa_identity);
	pop_string(msg, &msg->add_conn.xauth_identity);
	pop_string(msg, &msg->add_conn.algorithms.ike);
	pop_string(msg, &msg->add_conn.algorithms.esp);
	pop_string(msg, &msg->add_conn.algorithms.ah);
	pop_string(msg, &msg->add_conn.ikeme.mediated_by);
	pop_string(msg, &msg->add_conn.ikeme.peerid);
	DBG_OPT("  eap_identity=%s", msg->add_conn.eap_identity);
	DBG_OPT("  aaa_identity=%s", msg->add_conn.aaa_identity);
	DBG_OPT("  xauth_identity=%s", msg->add_conn.xauth_identity);
	DBG_OPT("  ike=%s", msg->add_conn.algorithms.ike);
	DBG_OPT("  esp=%s", msg->add_conn.algorithms.esp);
	DBG_OPT("  ah=%s", msg->add_conn.algorithms.ah);
	DBG_OPT("  dpddelay=%d", msg->add_conn.dpd.delay);
	DBG_OPT("  dpdtimeout=%d", msg->add_conn.dpd.timeout);
	DBG_OPT("  dpdaction=%d", msg->add_conn.dpd.action);
	DBG_OPT("  closeaction=%d", msg->add_conn.close_action);
	DBG_OPT("  sha256_96=%s", msg->add_conn.sha256_96 ? "yes" : "no");
	DBG_OPT("  mediation=%s", msg->add_conn.ikeme.mediation ? "yes" : "no");
	DBG_OPT("  mediated_by=%s", msg->add_conn.ikeme.mediated_by);
	DBG_OPT("  me_peerid=%s", msg->add_conn.ikeme.peerid);
	DBG_OPT("  keyexchange=ikev%u", msg->add_conn.version);

	this->config->add(this->config, msg);
	this->attribute->add_dns(this->attribute, msg);
	this->handler->add_attributes(this->handler, msg);
}
