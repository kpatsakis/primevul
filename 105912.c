PHP_FUNCTION(pg_send_prepare)
{
	zval *pgsql_link;
	char *query, *stmtname;
	int stmtname_len, query_len, id = -1;
	PGconn *pgsql;
	PGresult *res;
	int leftover = 0;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rss", &pgsql_link, &stmtname, &stmtname_len, &query, &query_len) == FAILURE) {
		return;
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 1)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}
	while ((res = PQgetResult(pgsql))) {
		PQclear(res);
		leftover = 1;
	}
	if (leftover) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "There are results on this connection. Call pg_get_result() until it returns FALSE");
	}
	if (!PQsendPrepare(pgsql, stmtname, query, 0, NULL)) {
		if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
			PQreset(pgsql);
		}
		if (!PQsendPrepare(pgsql, stmtname, query, 0, NULL)) {
			RETURN_FALSE;
		}
	}
	/* Wait to finish sending buffer */
	while ((ret = PQflush(pgsql))) {
		if (ret == -1) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Could not empty postgres send buffer");
			break;
		}
		usleep(10000);
	}
	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot set connection to blocking mode");
	}
	RETURN_TRUE;
}
