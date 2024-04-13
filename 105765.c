MYSQLND_METHOD(mysqlnd_conn_data, query)(MYSQLND_CONN_DATA * conn, const char * query, unsigned int query_len TSRMLS_DC)
{
	size_t this_func = STRUCT_OFFSET(struct st_mysqlnd_conn_data_methods, query);
	enum_func_status ret = FAIL;
	DBG_ENTER("mysqlnd_conn_data::query");
	DBG_INF_FMT("conn=%llu query=%s", conn->thread_id, query);

	if (PASS == conn->m->local_tx_start(conn, this_func TSRMLS_CC)) {
		if (PASS == conn->m->send_query(conn, query, query_len TSRMLS_CC) &&
			PASS == conn->m->reap_query(conn TSRMLS_CC))
		{
			ret = PASS;
			if (conn->last_query_type == QUERY_UPSERT && conn->upsert_status->affected_rows) {
				MYSQLND_INC_CONN_STATISTIC_W_VALUE(conn->stats, STAT_ROWS_AFFECTED_NORMAL, conn->upsert_status->affected_rows);
			}
		}
		conn->m->local_tx_end(conn, this_func, ret TSRMLS_CC);
	}
	DBG_RETURN(ret);
}
