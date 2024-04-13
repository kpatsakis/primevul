MYSQLND_METHOD_PRIVATE(mysqlnd_conn_data, dtor)(MYSQLND_CONN_DATA * conn TSRMLS_DC)
{
	DBG_ENTER("mysqlnd_conn_data::dtor");
	DBG_INF_FMT("conn=%llu", conn->thread_id);

	conn->m->free_contents(conn TSRMLS_CC);
	conn->m->free_options(conn TSRMLS_CC);

	if (conn->net) {
		mysqlnd_net_free(conn->net, conn->stats, conn->error_info TSRMLS_CC);
		conn->net = NULL;
	}

	if (conn->protocol) {
		mysqlnd_protocol_free(conn->protocol TSRMLS_CC);
		conn->protocol = NULL;
	}

	if (conn->stats) {
		mysqlnd_stats_end(conn->stats);
	}

	mnd_pefree(conn, conn->persistent);

	DBG_VOID_RETURN;
}
