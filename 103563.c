PHP_FUNCTION(pg_close)
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

	if (id==-1) { /* explicit resource number */
		zend_list_close(Z_RES_P(pgsql_link));
	}

	if (id!=-1
		|| (pgsql_link && Z_RES_P(pgsql_link) == PGG(default_link))) {
		zend_list_close(PGG(default_link));
		PGG(default_link) = NULL;
	}

	RETURN_TRUE;
}
