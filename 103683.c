static void php_pgsql_get_field_info(INTERNAL_FUNCTION_PARAMETERS, int entry_type)
{
	zval *result;
	zend_long field;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	Oid oid;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &result, &field) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	
	if (field < 0 || field >= PQnfields(pgsql_result)) {
		php_error_docref(NULL, E_WARNING, "Bad field offset specified");
		RETURN_FALSE;
	}

	switch (entry_type) {
		case PHP_PG_FIELD_NAME:
			RETURN_STRING(PQfname(pgsql_result, (int)field));
			break;
		case PHP_PG_FIELD_SIZE:
			RETURN_LONG(PQfsize(pgsql_result, (int)field));
			break;
		case PHP_PG_FIELD_TYPE: {
				char *name = get_field_name(pg_result->conn, PQftype(pgsql_result, (int)field), &EG(regular_list));
				RETVAL_STRING(name);
				efree(name);
			}
			break;
		case PHP_PG_FIELD_TYPE_OID:
			
			oid = PQftype(pgsql_result, (int)field);
#if UINT_MAX > ZEND_LONG_MAX
			if (oid > ZEND_LONG_MAX) {
				smart_str s = {0};
				smart_str_append_unsigned(&s, oid);
				smart_str_0(&s);
				RETURN_STR(s.s);
			} else
#endif
			{
				RETURN_LONG((zend_long)oid);
			}
			break;
		default:
			RETURN_FALSE;
	}
}
