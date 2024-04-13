PHP_FUNCTION(pg_fetch_result)
{
	zval *result, *field=NULL;
	zend_long row;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	int field_offset, pgsql_row, argc = ZEND_NUM_ARGS();

	if (argc == 2) {
		if (zend_parse_parameters(argc, "rz", &result, &field) == FAILURE) {
			return;
		}
	} else {
		if (zend_parse_parameters(argc, "rlz", &result, &row, &field) == FAILURE) {
			return;
		}
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

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
		if (row < 0 || row >= PQntuples(pgsql_result)) {
			php_error_docref(NULL, E_WARNING, "Unable to jump to row %pd on PostgreSQL result index %pd",
							row, Z_LVAL_P(result));
			RETURN_FALSE;
		}
		pgsql_row = (int)row;
	}
	switch (Z_TYPE_P(field)) {
		case IS_STRING:
			field_offset = PQfnumber(pgsql_result, Z_STRVAL_P(field));
			if (field_offset < 0 || field_offset >= PQnfields(pgsql_result)) {
				php_error_docref(NULL, E_WARNING, "Bad column offset specified");
				RETURN_FALSE;
			}
			break;
		default:
			convert_to_long_ex(field);
			if (Z_LVAL_P(field) < 0 || Z_LVAL_P(field) >= PQnfields(pgsql_result)) {
				php_error_docref(NULL, E_WARNING, "Bad column offset specified");
				RETURN_FALSE;
			}
			field_offset = (int)Z_LVAL_P(field);
			break;
	}

	if (PQgetisnull(pgsql_result, pgsql_row, field_offset)) {
		RETVAL_NULL();
	} else {
		RETVAL_STRINGL(PQgetvalue(pgsql_result, pgsql_row, field_offset),
				PQgetlength(pgsql_result, pgsql_row, field_offset));
	}
}
