PHP_FUNCTION(pg_flush)
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;
	int ret;
	int is_non_blocking;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &pgsql_link) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	is_non_blocking = PQisnonblocking(pgsql);

	if (is_non_blocking == 0 && PQ_SETNONBLOCKING(pgsql, 1) == -1) {
		php_error_docref(NULL, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}

	ret = PQflush(pgsql);

	if (is_non_blocking == 0 && PQ_SETNONBLOCKING(pgsql, 0) == -1) {
		php_error_docref(NULL, E_NOTICE, "Failed resetting connection to blocking mode");
	}

	switch (ret) {
		case 0: RETURN_TRUE; break;
		case 1: RETURN_LONG(0); break;
		default: RETURN_FALSE;
	}
}
