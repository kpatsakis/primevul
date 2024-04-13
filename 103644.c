PHP_FUNCTION(pg_insert)
{
	zval *pgsql_link, *values;
	char *table;
	size_t table_len;
	zend_ulong option = PGSQL_DML_EXEC, return_sql;
	PGconn *pg_link;
	PGresult *pg_result;
	ExecStatusType status;
	pgsql_result_handle *pgsql_handle;
	zend_string *sql = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "rsa|l",
							  &pgsql_link, &table, &table_len, &values, &option) == FAILURE) {
		return;
	}
	if (option & ~(PGSQL_CONV_OPTS|PGSQL_DML_NO_CONV|PGSQL_DML_EXEC|PGSQL_DML_ASYNC|PGSQL_DML_STRING|PGSQL_DML_ESCAPE)) {
		php_error_docref(NULL, E_WARNING, "Invalid option is specified");
		RETURN_FALSE;
	}
	
	ZEND_FETCH_RESOURCE2(pg_link, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (php_pgsql_flush_query(pg_link)) {
		php_error_docref(NULL, E_NOTICE, "Detected unhandled result(s) in connection");
	}
	return_sql = option & PGSQL_DML_STRING;
	if (option & PGSQL_DML_EXEC) {
		/* return resource when executed */
		option = option & ~PGSQL_DML_EXEC;
		if (php_pgsql_insert(pg_link, table, values, option|PGSQL_DML_STRING, &sql) == FAILURE) {
			RETURN_FALSE;
		}
		pg_result = PQexec(pg_link, sql->val);
		if ((PGG(auto_reset_persistent) & 2) && PQstatus(pg_link) != CONNECTION_OK) {
			PQclear(pg_result);
			PQreset(pg_link);
			pg_result = PQexec(pg_link, sql->val);
		}
		efree(sql);

		if (pg_result) {
			status = PQresultStatus(pg_result);
		} else {
			status = (ExecStatusType) PQstatus(pg_link);
		}

		switch (status) {
			case PGRES_EMPTY_QUERY:
			case PGRES_BAD_RESPONSE:
			case PGRES_NONFATAL_ERROR:
			case PGRES_FATAL_ERROR:
				PHP_PQ_ERROR("Query failed: %s", pg_link);
				PQclear(pg_result);
				RETURN_FALSE;
				break;
			case PGRES_COMMAND_OK: /* successful command that did not return rows */
			default:
				if (pg_result) {
					pgsql_handle = (pgsql_result_handle *) emalloc(sizeof(pgsql_result_handle));
					pgsql_handle->conn = pg_link;
					pgsql_handle->result = pg_result;
					pgsql_handle->row = 0;
					ZEND_REGISTER_RESOURCE(return_value, pgsql_handle, le_result);
					return;
				} else {
					PQclear(pg_result);
					RETURN_FALSE;
				}
			break;
		}
	} else if (php_pgsql_insert(pg_link, table, values, option, &sql) == FAILURE) {
		RETURN_FALSE;
	}
	if (return_sql) {
		RETURN_STR(sql);
		return;
	}
	RETURN_TRUE;
}
