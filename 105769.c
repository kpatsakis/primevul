MYSQLND_METHOD(mysqlnd_conn_data, list_method)(MYSQLND_CONN_DATA * conn, const char * query, const char *achtung_wild, char *par1 TSRMLS_DC)
{
	size_t this_func = STRUCT_OFFSET(struct st_mysqlnd_conn_data_methods, list_method);
	char * show_query = NULL;
	size_t show_query_len;
	MYSQLND_RES * result = NULL;

	DBG_ENTER("mysqlnd_conn_data::list_method");
	DBG_INF_FMT("conn=%llu query=%s wild=%u", conn->thread_id, query, achtung_wild);

	if (PASS == conn->m->local_tx_start(conn, this_func TSRMLS_CC)) {
		if (par1) {
			if (achtung_wild) {
				show_query_len = mnd_sprintf(&show_query, 0, query, par1, achtung_wild);
			} else {
				show_query_len = mnd_sprintf(&show_query, 0, query, par1);
			}
		} else {
			if (achtung_wild) {
				show_query_len = mnd_sprintf(&show_query, 0, query, achtung_wild);
			} else {
				show_query_len = strlen(show_query = (char *)query);
			}
		}

		if (PASS == conn->m->query(conn, show_query, show_query_len TSRMLS_CC)) {
			result = conn->m->store_result(conn TSRMLS_CC);
		}
		if (show_query != query) {
			mnd_sprintf_free(show_query);
		}
		conn->m->local_tx_end(conn, this_func, result == NULL? FAIL:PASS TSRMLS_CC);
	}
	DBG_RETURN(result);
}
