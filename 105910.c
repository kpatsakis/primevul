PHP_FUNCTION(pg_send_query)
{
	zval *pgsql_link;
	char *query;
	int len;
	int id = -1;
	PGconn *pgsql;
	PGresult *res;
	int leftover = 0;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs",
							  &pgsql_link, &query, &len) == FAILURE) {
		return;
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
	if (!PQsendQuery(pgsql, query)) {
		if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
			PQreset(pgsql);
		}
		if (!PQsendQuery(pgsql, query)) {
			RETURN_FALSE;
		}
	}
	/* Wait to finish sending buffer */
	while ((ret = PQflush(pgsql))) {
		if (ret == -1) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Could not empty PostgreSQL send buffer");
			break;
		}
		usleep(10000);
	}
	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot set connection to blocking mode");
	}
	RETURN_TRUE;
}
