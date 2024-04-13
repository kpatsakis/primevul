MYSQLND_METHOD(mysqlnd_conn_data, send_query)(MYSQLND_CONN_DATA * conn, const char * query, unsigned int query_len TSRMLS_DC)
{
	size_t this_func = STRUCT_OFFSET(struct st_mysqlnd_conn_data_methods, send_query);
	enum_func_status ret = FAIL;
	DBG_ENTER("mysqlnd_conn_data::send_query");
	DBG_INF_FMT("conn=%llu query=%s", conn->thread_id, query);
	DBG_INF_FMT("conn->server_status=%u", conn->upsert_status->server_status);

	if (PASS == conn->m->local_tx_start(conn, this_func TSRMLS_CC)) {
		ret = conn->m->simple_command(conn, COM_QUERY, (zend_uchar *) query, query_len,
											 PROT_LAST /* we will handle the OK packet*/,
											 FALSE, FALSE TSRMLS_CC);
		if (PASS == ret) {
			CONN_SET_STATE(conn, CONN_QUERY_SENT);
		}
		conn->m->local_tx_end(conn, this_func, ret TSRMLS_CC);
	}
	DBG_INF_FMT("conn->server_status=%u", conn->upsert_status->server_status);
	DBG_RETURN(ret);
}
