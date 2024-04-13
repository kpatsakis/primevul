PHP_FUNCTION(pg_field_num)
{
	zval *result;
	char *field;
	int field_len;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &result, &field, &field_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, &result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;

	Z_LVAL_P(return_value) = PQfnumber(pgsql_result, field);
	Z_TYPE_P(return_value) = IS_LONG;
}
