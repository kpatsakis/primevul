PHP_FUNCTION(pg_copy_from)
{
	zval *pgsql_link = NULL, *pg_rows;
	zval *tmp;
	char *table_name, *pg_delim = NULL, *pg_null_as = NULL;
	size_t  table_name_len, pg_delim_len, pg_null_as_len;
	int  pg_null_as_free = 0;
	char *query;
	int id = -1;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "rsa|ss",
							  &pgsql_link, &table_name, &table_name_len, &pg_rows,
							  &pg_delim, &pg_delim_len, &pg_null_as, &pg_null_as_len) == FAILURE) {
		return;
	}
	if (!pg_delim) {
		pg_delim = "\t";
	}
	if (!pg_null_as) {
		pg_null_as = estrdup("\\\\N");
		pg_null_as_free = 1;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	spprintf(&query, 0, "COPY %s FROM STDIN DELIMITERS E'%c' WITH NULL AS E'%s'", table_name, *pg_delim, pg_null_as);
	while ((pgsql_result = PQgetResult(pgsql))) {
		PQclear(pgsql_result);
	}
	pgsql_result = PQexec(pgsql, query);

	if (pg_null_as_free) {
		efree(pg_null_as);
	}
	efree(query);

	if (pgsql_result) {
		status = PQresultStatus(pgsql_result);
	} else {
		status = (ExecStatusType) PQstatus(pgsql);
	}

	switch (status) {
		case PGRES_COPY_IN:
			if (pgsql_result) {
				int command_failed = 0;
				PQclear(pgsql_result);
#if HAVE_PQPUTCOPYDATA
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(pg_rows), tmp) {
					convert_to_string_ex(tmp);
					query = (char *)emalloc(Z_STRLEN_P(tmp) + 2);
					strlcpy(query, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp) + 2);
					if(Z_STRLEN_P(tmp) > 0 && *(query + Z_STRLEN_P(tmp) - 1) != '\n') {
						strlcat(query, "\n", Z_STRLEN_P(tmp) + 2);
					}
					if (PQputCopyData(pgsql, query, (int)strlen(query)) != 1) {
						efree(query);
						PHP_PQ_ERROR("copy failed: %s", pgsql);
						RETURN_FALSE;
					}
					efree(query);
				} ZEND_HASH_FOREACH_END();
				if (PQputCopyEnd(pgsql, NULL) != 1) {
					PHP_PQ_ERROR("putcopyend failed: %s", pgsql);
					RETURN_FALSE;
				}
#else
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(pg_rows), tmp) {
					convert_to_string_ex(tmp);
					query = (char *)emalloc(Z_STRLEN_P(tmp) + 2);
					strlcpy(query, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp) + 2);
					if(Z_STRLEN_P(tmp) > 0 && *(query + Z_STRLEN_P(tmp) - 1) != '\n') {
						strlcat(query, "\n", Z_STRLEN_P(tmp) + 2);
					}
					if (PQputline(pgsql, query)==EOF) {
						efree(query);
						PHP_PQ_ERROR("copy failed: %s", pgsql);
						RETURN_FALSE;
					}
					efree(query);
				} ZEND_HASH_FOREACH_END();
				if (PQputline(pgsql, "\\.\n") == EOF) {
					PHP_PQ_ERROR("putline failed: %s", pgsql);
					RETURN_FALSE;
				}
				if (PQendcopy(pgsql)) {
					PHP_PQ_ERROR("endcopy failed: %s", pgsql);
					RETURN_FALSE;
				}
#endif
				while ((pgsql_result = PQgetResult(pgsql))) {
					if (PGRES_COMMAND_OK != PQresultStatus(pgsql_result)) {
						PHP_PQ_ERROR("Copy command failed: %s", pgsql);
						command_failed = 1;
					}
					PQclear(pgsql_result);
				}
				if (command_failed) {
					RETURN_FALSE;
				}
			} else {
				PQclear(pgsql_result);
				RETURN_FALSE;
			}
			RETURN_TRUE;
			break;
		default:
			PQclear(pgsql_result);
			PHP_PQ_ERROR("Copy command failed: %s", pgsql);
			RETURN_FALSE;
			break;
	}
}
