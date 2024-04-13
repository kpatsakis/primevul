PHP_FUNCTION(pg_select)
{
	zval *pgsql_link, *ids;
	char *table;
	size_t table_len;
	zend_ulong option = PGSQL_DML_EXEC;
	PGconn *pg_link;
	zend_string *sql = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "rsa|l",
							  &pgsql_link, &table, &table_len, &ids, &option) == FAILURE) {
		return;
	}
	if (option & ~(PGSQL_CONV_FORCE_NULL|PGSQL_DML_NO_CONV|PGSQL_DML_EXEC|PGSQL_DML_ASYNC|PGSQL_DML_STRING|PGSQL_DML_ESCAPE)) {
		php_error_docref(NULL, E_WARNING, "Invalid option is specified");
		RETURN_FALSE;
	}
	
	ZEND_FETCH_RESOURCE2(pg_link, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (php_pgsql_flush_query(pg_link)) {
		php_error_docref(NULL, E_NOTICE, "Detected unhandled result(s) in connection");
	}
	array_init(return_value);
	if (php_pgsql_select(pg_link, table, ids, return_value, option, &sql) == FAILURE) {
		zval_ptr_dtor(return_value);
		RETURN_FALSE;
	}
	if (option & PGSQL_DML_STRING) {
		zval_ptr_dtor(return_value);
		RETURN_STR(sql);
	}
	return;
}
