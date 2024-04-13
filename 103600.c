PHP_FUNCTION(pg_untrace)
{
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;
	
	if (zend_parse_parameters(argc, "|r", &pgsql_link) == FAILURE) {
		return;
	}

	if (argc == 0) { 
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	PQuntrace(pgsql);
	RETURN_TRUE;
}
