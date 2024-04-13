PHP_FUNCTION(pg_parameter_status)
{
	zval *pgsql_link;
	int id;
	PGconn *pgsql;
	char *param;
	int len;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "rs", &pgsql_link, &param, &len) == SUCCESS) {
		id = -1;
	} else if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &param, &len) == SUCCESS) {
		pgsql_link = NULL;
		id = PGG(default_link);
	} else {
		RETURN_FALSE;
	}
	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	param = (char*)PQparameterStatus(pgsql, param);
	if (param) {
		RETURN_STRING(param, 1);
	} else {
		RETURN_FALSE;
	}
}
