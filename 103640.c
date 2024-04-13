PHP_FUNCTION(pg_consume_input)
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &pgsql_link) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	RETURN_BOOL(PQconsumeInput(pgsql));
}
