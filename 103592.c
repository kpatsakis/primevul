PHP_FUNCTION(pg_fetch_all)
{
	zval *result;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &result) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	array_init(return_value);
	if (php_pgsql_result2array(pgsql_result, return_value) == FAILURE) {
		zval_dtor(return_value);
		RETURN_FALSE;
	}
}
