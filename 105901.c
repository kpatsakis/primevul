PHP_FUNCTION(pg_copy_from)
{
	zval *pgsql_link = NULL, *pg_rows;
	zval **tmp;
	char *table_name, *pg_delim = NULL, *pg_null_as = NULL;
	int  table_name_len, pg_delim_len, pg_null_as_len;
	int  pg_null_as_free = 0;
	char *query;
	HashPosition pos;
	int id = -1;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "rsa|ss",
							  &pgsql_link, &table_name, &table_name_len, &pg_rows,
							  &pg_delim, &pg_delim_len, &pg_null_as, &pg_null_as_len) == FAILURE) {
		return;
	}
	if (!pg_delim) {
		pg_delim = "\t";
	}
	if (!pg_null_as) {
		pg_null_as = safe_estrdup("\\\\N");
		pg_null_as_free = 1;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

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
				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(pg_rows), &pos);
#if HAVE_PQPUTCOPYDATA
				while (zend_hash_get_current_data_ex(Z_ARRVAL_P(pg_rows), (void **) &tmp, &pos) == SUCCESS) {
					zval *value;
					ALLOC_ZVAL(value);
					INIT_PZVAL_COPY(value, *tmp);
					zval_copy_ctor(value);
					convert_to_string_ex(&value);
					query = (char *)emalloc(Z_STRLEN_P(value) + 2);
					strlcpy(query, Z_STRVAL_P(value), Z_STRLEN_P(value) + 2);
					if(Z_STRLEN_P(value) > 0 && *(query + Z_STRLEN_P(value) - 1) != '\n') {
						strlcat(query, "\n", Z_STRLEN_P(value) + 2);
					}
					if (PQputCopyData(pgsql, query, strlen(query)) != 1) {
						efree(query);
						zval_dtor(value);
						efree(value);
						PHP_PQ_ERROR("copy failed: %s", pgsql);
						RETURN_FALSE;
					}
					efree(query);
					zval_dtor(value);
					efree(value);
					zend_hash_move_forward_ex(Z_ARRVAL_P(pg_rows), &pos);
				}
				if (PQputCopyEnd(pgsql, NULL) != 1) {
					PHP_PQ_ERROR("putcopyend failed: %s", pgsql);
					RETURN_FALSE;
				}
#else
				while (zend_hash_get_current_data_ex(Z_ARRVAL_P(pg_rows), (void **) &tmp, &pos) == SUCCESS) {
					zval *value;
					ALLOC_ZVAL(value);
					INIT_PZVAL_COPY(value, *tmp);
					zval_copy_ctor(value);
					convert_to_string_ex(&value);
					query = (char *)emalloc(Z_STRLEN_P(value) + 2);
					strlcpy(query, Z_STRVAL_P(value), Z_STRLEN_P(value) + 2);
					if(Z_STRLEN_P(value) > 0 && *(query + Z_STRLEN_P(value) - 1) != '\n') {
						strlcat(query, "\n", Z_STRLEN_P(value) + 2);
					}
					if (PQputline(pgsql, query)==EOF) {
						efree(query);
						zval_dtor(value);
						efree(value);
						PHP_PQ_ERROR("copy failed: %s", pgsql);
						RETURN_FALSE;
					}
					efree(query);
					zval_dtor(value);
					efree(value);
					zend_hash_move_forward_ex(Z_ARRVAL_P(pg_rows), &pos);
				}
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
