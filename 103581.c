PHP_FUNCTION(pg_field_table)
{
	zval *result;
	pgsql_result_handle *pg_result;
	zend_long fnum = -1;
	zend_bool return_oid = 0;
	Oid oid;
	smart_str hash_key = {0};
	char *table_name;
	zend_resource *field_table;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl|b", &result, &fnum, &return_oid) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	if (fnum < 0 || fnum >= PQnfields(pg_result->result)) {
		php_error_docref(NULL, E_WARNING, "Bad field offset specified");
		RETURN_FALSE;
	}

	oid = PQftable(pg_result->result, (int)fnum);

	if (InvalidOid == oid) {
		RETURN_FALSE;
	}

	if (return_oid) {
#if UINT_MAX > ZEND_LONG_MAX /* Oid is unsigned int, we don't need this code, where LONG is wider */
		if (oid > ZEND_LONG_MAX) {
			smart_str oidstr = {0};
			smart_str_append_unsigned(&oidstr, oid);
			smart_str_0(&oidstr);
			RETURN_STR(oidstr.s);
		} else
#endif
			RETURN_LONG((zend_long)oid);
	}

	/* try to lookup the table name in the resource list */
	smart_str_appends(&hash_key, "pgsql_table_oid_");
	smart_str_append_unsigned(&hash_key, oid);
	smart_str_0(&hash_key);

	if ((field_table = zend_hash_find_ptr(&EG(regular_list), hash_key.s)) != NULL) {
		smart_str_free(&hash_key);
		RETURN_STRING((char *)field_table->ptr);
	} else { /* Not found, lookup by querying PostgreSQL system tables */
		PGresult *tmp_res;
		smart_str querystr = {0};
		zend_resource new_field_table;

		smart_str_appends(&querystr, "select relname from pg_class where oid=");
		smart_str_append_unsigned(&querystr, oid);
		smart_str_0(&querystr);

		if ((tmp_res = PQexec(pg_result->conn, querystr.s->val)) == NULL || PQresultStatus(tmp_res) != PGRES_TUPLES_OK) {
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

		new_field_table.type = le_string;
		new_field_table.ptr = estrdup(table_name);
		zend_hash_update_mem(&EG(regular_list), hash_key.s, (void *)&new_field_table, sizeof(zend_resource));

		smart_str_free(&hash_key);
		PQclear(tmp_res);
		RETURN_STRING(table_name);
	}

}
