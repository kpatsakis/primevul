static inline int build_assignment_string(PGconn *pg_link, smart_str *querystr, HashTable *ht, int where_cond, const char *pad, int pad_len, zend_ulong opt) /* {{{ */
{
	char *tmp;
	char buf[256];
	zend_ulong num_idx;
	zend_string *fld;
	zval *val;

	ZEND_HASH_FOREACH_KEY_VAL(ht, num_idx, fld, val) {
		if (fld == NULL) {
			php_error_docref(NULL, E_NOTICE, "Expects associative array for values to be inserted");
			return -1;
		}
		if (opt & PGSQL_DML_ESCAPE) {
			tmp = PGSQLescapeIdentifier(pg_link, fld->val, fld->len + 1);
			smart_str_appends(querystr, tmp);
			PGSQLfree(tmp);
		} else {
			smart_str_appendl(querystr, fld->val, fld->len);
		}
		if (where_cond && (Z_TYPE_P(val) == IS_TRUE || Z_TYPE_P(val) == IS_FALSE || (Z_TYPE_P(val) == IS_STRING && !strcmp(Z_STRVAL_P(val), "NULL")))) {
			smart_str_appends(querystr, " IS ");
		} else {
			smart_str_appendc(querystr, '=');
		}

		switch (Z_TYPE_P(val)) {
			case IS_STRING:
				if (opt & PGSQL_DML_ESCAPE) {
					size_t new_len;
					tmp = (char *)safe_emalloc(Z_STRLEN_P(val), 2, 1);
					new_len = PQescapeStringConn(pg_link, tmp, Z_STRVAL_P(val), Z_STRLEN_P(val), NULL);
					smart_str_appendc(querystr, '\'');
					smart_str_appendl(querystr, tmp, new_len);
					smart_str_appendc(querystr, '\'');
					efree(tmp);
				} else {
					smart_str_appendl(querystr, Z_STRVAL_P(val), Z_STRLEN_P(val));
				}
				break;
			case IS_LONG:
				smart_str_append_long(querystr, Z_LVAL_P(val));
				break;
			case IS_DOUBLE:
				smart_str_appendl(querystr, buf, MIN(snprintf(buf, sizeof(buf), "%F", Z_DVAL_P(val)), sizeof(buf)-1));
				break;
			case IS_NULL:
				smart_str_appendl(querystr, "NULL", sizeof("NULL")-1);
				break;
			default:
				php_error_docref(NULL, E_WARNING, "Expects scaler values. type=%d", Z_TYPE_P(val));
				return -1;
		}
		smart_str_appendl(querystr, pad, pad_len);
	} ZEND_HASH_FOREACH_END();
	if (querystr->s) {
		querystr->s->len -= pad_len;
	}

	return 0;
} 
/* }}} */
