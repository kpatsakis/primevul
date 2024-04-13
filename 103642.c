PHP_FUNCTION(pg_meta_data)
{
	zval *pgsql_link;
	char *table_name;
	size_t table_name_len;
	zend_bool extended=0;
	PGconn *pgsql;
	int id = -1;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rs|b",
							  &pgsql_link, &table_name, &table_name_len, &extended) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	array_init(return_value);
	if (php_pgsql_meta_data(pgsql, table_name, return_value, extended) == FAILURE) {
		zval_dtor(return_value); /* destroy array */
		RETURN_FALSE;
	}
}
