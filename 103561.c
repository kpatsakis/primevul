PHP_FUNCTION(pg_connect_poll)
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &pgsql_link) == FAILURE) {
		return;
	}

	if (pgsql_link == NULL) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	ret = PQconnectPoll(pgsql);

	RETURN_LONG(ret);
}
