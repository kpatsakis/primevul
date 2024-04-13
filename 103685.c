static void php_pgsql_get_result_info(INTERNAL_FUNCTION_PARAMETERS, int entry_type)
{
	zval *result;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &result) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;

	switch (entry_type) {
		case PHP_PG_NUM_ROWS:
			RETVAL_LONG(PQntuples(pgsql_result));
			break;
		case PHP_PG_NUM_FIELDS:
			RETVAL_LONG(PQnfields(pgsql_result));
			break;
		case PHP_PG_CMD_TUPLES:
#if HAVE_PQCMDTUPLES
			RETVAL_LONG(atoi(PQcmdTuples(pgsql_result)));
#else
			php_error_docref(NULL, E_WARNING, "Not supported under this build");
			RETVAL_LONG(0);
#endif
			break;
		default:
			RETURN_FALSE;
	}
}
