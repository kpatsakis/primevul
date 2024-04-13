void http_return_srv_error(struct stream *s, struct stream_interface *si)
{
	int err_type = si->err_type;

	/* set s->txn->status for http_error_message(s) */
	s->txn->status = 503;

	if (err_type & SI_ET_QUEUE_ABRT)
		http_server_error(s, si, SF_ERR_CLICL, SF_FINST_Q,
				  http_error_message(s));
	else if (err_type & SI_ET_CONN_ABRT)
		http_server_error(s, si, SF_ERR_CLICL, SF_FINST_C,
				  (s->txn->flags & TX_NOT_FIRST) ? NULL :
				  http_error_message(s));
	else if (err_type & SI_ET_QUEUE_TO)
		http_server_error(s, si, SF_ERR_SRVTO, SF_FINST_Q,
				  http_error_message(s));
	else if (err_type & SI_ET_QUEUE_ERR)
		http_server_error(s, si, SF_ERR_SRVCL, SF_FINST_Q,
				  http_error_message(s));
	else if (err_type & SI_ET_CONN_TO)
		http_server_error(s, si, SF_ERR_SRVTO, SF_FINST_C,
				  (s->txn->flags & TX_NOT_FIRST) ? NULL :
				  http_error_message(s));
	else if (err_type & SI_ET_CONN_ERR)
		http_server_error(s, si, SF_ERR_SRVCL, SF_FINST_C,
				  (s->flags & SF_SRV_REUSED) ? NULL :
				  http_error_message(s));
	else if (err_type & SI_ET_CONN_RES)
		http_server_error(s, si, SF_ERR_RESOURCE, SF_FINST_C,
				  (s->txn->flags & TX_NOT_FIRST) ? NULL :
				  http_error_message(s));
	else { /* SI_ET_CONN_OTHER and others */
		s->txn->status = 500;
		http_server_error(s, si, SF_ERR_INTERNAL, SF_FINST_C,
				  http_error_message(s));
	}
}
