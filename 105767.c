MYSQLND_METHOD(mysqlnd_conn_data, reap_query)(MYSQLND_CONN_DATA * conn TSRMLS_DC)
{
	size_t this_func = STRUCT_OFFSET(struct st_mysqlnd_conn_data_methods, reap_query);
	enum_mysqlnd_connection_state state = CONN_GET_STATE(conn);
	enum_func_status ret = FAIL;
	DBG_ENTER("mysqlnd_conn_data::reap_query");
	DBG_INF_FMT("conn=%llu", conn->thread_id);

	DBG_INF_FMT("conn->server_status=%u", conn->upsert_status->server_status);
	if (PASS == conn->m->local_tx_start(conn, this_func TSRMLS_CC)) {
		if (state <= CONN_READY || state == CONN_QUIT_SENT) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Connection not opened, clear or has been closed");
			DBG_ERR_FMT("Connection not opened, clear or has been closed. State=%u", state);
			DBG_RETURN(ret);
		}
		ret = conn->m->query_read_result_set_header(conn, NULL TSRMLS_CC);

		conn->m->local_tx_end(conn, this_func, ret TSRMLS_CC);
	}
	DBG_INF_FMT("conn->server_status=%u", conn->upsert_status->server_status);
	DBG_RETURN(ret);
}
