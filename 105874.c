PHP_FUNCTION(pg_field_table)
{
	zval *result;
	pgsql_result_handle *pg_result;
	long fnum = -1;
	zend_bool return_oid = 0;
	Oid oid;
	smart_str hash_key = {0};
	char *table_name;
	zend_rsrc_list_entry *field_table;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl|b", &result, &fnum, &return_oid) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, &result, -1, "PostgreSQL result", le_result);

	if (fnum < 0 || fnum >= PQnfields(pg_result->result)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Bad field offset specified");
		RETURN_FALSE;
	}

	oid = PQftable(pg_result->result, fnum);

	if (InvalidOid == oid) {
		RETURN_FALSE;
	}

	if (return_oid) {
#if UINT_MAX > LONG_MAX /* Oid is unsigned int, we don't need this code, where LONG is wider */
		if (oid > LONG_MAX) {
			smart_str oidstr = {0};
			smart_str_append_unsigned(&oidstr, oid);
			smart_str_0(&oidstr);
			RETURN_STRINGL(oidstr.c, oidstr.len, 0);
		} else
#endif
			RETURN_LONG((long)oid);
	}

	/* try to lookup the table name in the resource list */
	smart_str_appends(&hash_key, "pgsql_table_oid_");
	smart_str_append_unsigned(&hash_key, oid);
	smart_str_0(&hash_key);

	if (zend_hash_find(&EG(regular_list), hash_key.c, hash_key.len+1, (void **) &field_table) == SUCCESS) {
		smart_str_free(&hash_key);
		RETURN_STRING((char *)field_table->ptr, 1);
	} else { /* Not found, lookup by querying PostgreSQL system tables */
		PGresult *tmp_res;
		smart_str querystr = {0};
		zend_rsrc_list_entry new_field_table;

		smart_str_appends(&querystr, "select relname from pg_class where oid=");
		smart_str_append_unsigned(&querystr, oid);
		smart_str_0(&querystr);

		if ((tmp_res = PQexec(pg_result->conn, querystr.c)) == NULL || PQresultStatus(tmp_res) != PGRES_TUPLES_OK) {
			if (tmp_res) {
				PQclear(tmp_res);
			}
			smart_str_free(&querystr);
			smart_str_free(&hash_key);
			RETURN_FALSE;
		}

		smart_str_free(&querystr);

		if ((table_name = PQgetvalue(tmp_res, 0, 0)) == NULL) {
			PQclear(tmp_res);
			smart_str_free(&hash_key);
			RETURN_FALSE;
		}

		Z_TYPE(new_field_table) = le_string;
		new_field_table.ptr = estrdup(table_name);
		zend_hash_update(&EG(regular_list), hash_key.c, hash_key.len+1, (void *) &new_field_table, sizeof(zend_rsrc_list_entry), NULL);

		smart_str_free(&hash_key);
		PQclear(tmp_res);
		RETURN_STRING(table_name, 1);
	}

}
