PHP_FUNCTION(pg_parameter_status)
{
	zval *pgsql_link;
	int id;
	PGconn *pgsql;
	char *param;
	size_t len;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "rs", &pgsql_link, &param, &len) == SUCCESS) {
		id = -1;
	} else if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &param, &len) == SUCCESS) {
		pgsql_link = NULL;
		id = FETCH_DEFAULT_LINK();
	} else {
		RETURN_FALSE;
	}
	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	param = (char*)PQparameterStatus(pgsql, param);
	if (param) {
		RETURN_STRING(param);
	} else {
		RETURN_FALSE;
	}
}
