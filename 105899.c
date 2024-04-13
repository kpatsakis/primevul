PHP_FUNCTION(pg_put_line)
{
	char *query;
	zval *pgsql_link = NULL;
	int query_len, id = -1;
	PGconn *pgsql;
	int result = 0, argc = ZEND_NUM_ARGS();

	if (argc == 1) {
		if (zend_parse_parameters(argc TSRMLS_CC, "s", &query, &query_len) == FAILURE) {
			return;
		}
		id = PGG(default_link);
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc TSRMLS_CC, "rs", &pgsql_link, &query, &query_len) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}	

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	result = PQputline(pgsql, query);
	if (result==EOF) {
		PHP_PQ_ERROR("Query failed: %s", pgsql);
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
