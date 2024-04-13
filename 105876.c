PHP_FUNCTION(pg_fetch_result)
{
	zval *result, **field=NULL;
	long row;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	int field_offset, pgsql_row, argc = ZEND_NUM_ARGS();

	if (argc == 2) {
		if (zend_parse_parameters(argc TSRMLS_CC, "rZ", &result, &field) == FAILURE) {
			return;
		}
	} else {
		if (zend_parse_parameters(argc TSRMLS_CC, "rlZ", &result, &row, &field) == FAILURE) {
			return;
		}
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, &result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	if (argc == 2) {
		if (pg_result->row < 0) {
			pg_result->row = 0;
		}
		pgsql_row = pg_result->row;
		if (pgsql_row >= PQntuples(pgsql_result)) {
			RETURN_FALSE;
		}
	} else {
		pgsql_row = row;
		if (pgsql_row < 0 || pgsql_row >= PQntuples(pgsql_result)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to jump to row %ld on PostgreSQL result index %ld",
							row, Z_LVAL_P(result));
			RETURN_FALSE;
		}
	}
	switch(Z_TYPE_PP(field)) {
		case IS_STRING:
			field_offset = PQfnumber(pgsql_result, Z_STRVAL_PP(field));
			break;
		default:
			convert_to_long_ex(field);
			field_offset = Z_LVAL_PP(field);
			break;
	}
	if (field_offset<0 || field_offset>=PQnfields(pgsql_result)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Bad column offset specified");
		RETURN_FALSE;
	}

	if (PQgetisnull(pgsql_result, pgsql_row, field_offset)) {
		Z_TYPE_P(return_value) = IS_NULL;
	} else {
		char *value = PQgetvalue(pgsql_result, pgsql_row, field_offset);
		int value_len = PQgetlength(pgsql_result, pgsql_row, field_offset);
		ZVAL_STRINGL(return_value, value, value_len, 1);
	}
}
