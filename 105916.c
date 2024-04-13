PHP_FUNCTION(pg_get_pid)
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "r",
								 &pgsql_link) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	RETURN_LONG(PQbackendPID(pgsql));
}
