PHP_FUNCTION(pg_free_result)
{
	zval *result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &result) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);
	if (Z_RES_P(result) == NULL) {
		RETURN_FALSE;
	}
	zend_list_close(Z_RES_P(result));
	RETURN_TRUE;
}
