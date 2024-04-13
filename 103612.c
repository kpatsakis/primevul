PHP_FUNCTION(pg_set_error_verbosity)
{
	zval *pgsql_link = NULL;
	zend_long verbosity;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;

	if (argc == 1) {
		if (zend_parse_parameters(argc, "l", &verbosity) == FAILURE) {
			return;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc, "rl", &pgsql_link, &verbosity) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}	

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (verbosity & (PQERRORS_TERSE|PQERRORS_DEFAULT|PQERRORS_VERBOSE)) {
		RETURN_LONG(PQsetErrorVerbosity(pgsql, verbosity));
	} else {
		RETURN_FALSE;
	}
}
