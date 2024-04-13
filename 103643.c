PHP_FUNCTION(pg_convert)
{
	zval *pgsql_link, *values;
	char *table_name;
	size_t table_name_len;
	zend_ulong option = 0;
	PGconn *pg_link;
	int id = -1;

	if (zend_parse_parameters(ZEND_NUM_ARGS(),
							  "rsa|l", &pgsql_link, &table_name, &table_name_len, &values, &option) == FAILURE) {
		return;
	}
	if (option & ~PGSQL_CONV_OPTS) {
		php_error_docref(NULL, E_WARNING, "Invalid option is specified");
		RETURN_FALSE;
	}
	if (!table_name_len) {
		php_error_docref(NULL, E_NOTICE, "Table name is invalid");
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pg_link, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (php_pgsql_flush_query(pg_link)) {
		php_error_docref(NULL, E_NOTICE, "Detected unhandled result(s) in connection");
	}
	array_init(return_value);
	if (php_pgsql_convert(pg_link, table_name, values, return_value, option) == FAILURE) {
		zval_dtor(return_value);
		RETURN_FALSE;
	}
}
