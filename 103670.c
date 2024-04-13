PHP_PGSQL_API int php_pgsql_convert(PGconn *pg_link, const char *table_name, const zval *values, zval *result, zend_ulong opt) 
{
	zend_string *field = NULL;
	zend_ulong num_idx = -1;
	zval meta, *def, *type, *not_null, *has_default, *is_enum, *val, new_val;
	int err = 0, skip_field;
	php_pgsql_data_type data_type;

	assert(pg_link != NULL);
	assert(Z_TYPE_P(values) == IS_ARRAY);
	assert(Z_TYPE_P(result) == IS_ARRAY);
	assert(!(opt & ~PGSQL_CONV_OPTS));

	if (!table_name) {
		return FAILURE;
	}

	array_init(&meta);
/* table_name is escaped by php_pgsql_meta_data */
	if (php_pgsql_meta_data(pg_link, table_name, &meta, 0) == FAILURE) {
		zval_ptr_dtor(&meta);
		return FAILURE;
	}

	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(values), num_idx, field, val) {
		skip_field = 0;
		ZVAL_NULL(&new_val);

		if (!err && field == NULL) {
			php_error_docref(NULL, E_WARNING, "Accepts only string key for values");
			err = 1;
		}

		if (!err && (def = zend_hash_find(Z_ARRVAL(meta), field)) == NULL) {
			php_error_docref(NULL, E_NOTICE, "Invalid field name (%s) in values", field->val);
			err = 1;
		}
		if (!err && (type = zend_hash_str_find(Z_ARRVAL_P(def), "type", sizeof("type") - 1)) == NULL) {
			php_error_docref(NULL, E_NOTICE, "Detected broken meta data. Missing 'type'");
			err = 1;
		}
		if (!err && (not_null = zend_hash_str_find(Z_ARRVAL_P(def), "not null", sizeof("not null") - 1)) == NULL) {
			php_error_docref(NULL, E_NOTICE, "Detected broken meta data. Missing 'not null'");
			err = 1;
		}
		if (!err && (has_default = zend_hash_str_find(Z_ARRVAL_P(def), "has default", sizeof("has default") - 1)) == NULL) {
			php_error_docref(NULL, E_NOTICE, "Detected broken meta data. Missing 'has default'");
			err = 1;
		}
		if (!err && (is_enum = zend_hash_str_find(Z_ARRVAL_P(def), "is enum", sizeof("is enum") - 1)) == NULL) {
			php_error_docref(NULL, E_NOTICE, "Detected broken meta data. Missing 'is enum'");
			err = 1;
		}
		if (!err && (Z_TYPE_P(val) == IS_ARRAY || Z_TYPE_P(val) == IS_OBJECT)) {
			php_error_docref(NULL, E_NOTICE, "Expects scalar values as field values");
			err = 1;
		}
		if (err) {
			break; /* break out for() */
		}

		convert_to_boolean(is_enum);
		if (Z_TYPE_P(is_enum) == IS_TRUE) {
			/* enums need to be treated like strings */
			data_type = PG_TEXT;
		} else {
			data_type = php_pgsql_get_data_type(Z_STRVAL_P(type), Z_STRLEN_P(type));
		}

		switch(data_type)
		{
			case PG_BOOL:
				switch (Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRING(&new_val, "NULL");
						}
						else {
							if (!strcmp(Z_STRVAL_P(val), "t") || !strcmp(Z_STRVAL_P(val), "T") ||
								!strcmp(Z_STRVAL_P(val), "y") || !strcmp(Z_STRVAL_P(val), "Y") ||
								!strcmp(Z_STRVAL_P(val), "true") || !strcmp(Z_STRVAL_P(val), "True") ||
								!strcmp(Z_STRVAL_P(val), "yes") || !strcmp(Z_STRVAL_P(val), "Yes") ||
								!strcmp(Z_STRVAL_P(val), "1")) {
								ZVAL_STRINGL(&new_val, "'t'", sizeof("'t'")-1);
							}
							else if (!strcmp(Z_STRVAL_P(val), "f") || !strcmp(Z_STRVAL_P(val), "F") ||
									 !strcmp(Z_STRVAL_P(val), "n") || !strcmp(Z_STRVAL_P(val), "N") ||
									 !strcmp(Z_STRVAL_P(val), "false") ||  !strcmp(Z_STRVAL_P(val), "False") ||
									 !strcmp(Z_STRVAL_P(val), "no") ||  !strcmp(Z_STRVAL_P(val), "No") ||
									 !strcmp(Z_STRVAL_P(val), "0")) {
								ZVAL_STRINGL(&new_val, "'f'", sizeof("'f'")-1);
							}
							else {
								php_error_docref(NULL, E_NOTICE, "Detected invalid value (%s) for PostgreSQL %s field (%s)", Z_STRVAL_P(val), Z_STRVAL_P(type), field->val);
								err = 1;
							}
						}
						break;

					case IS_LONG:
						if (Z_LVAL_P(val)) {
							ZVAL_STRINGL(&new_val, "'t'", sizeof("'t'")-1);
						}
						else {
							ZVAL_STRINGL(&new_val, "'f'", sizeof("'f'")-1);
						}
						break;

					case IS_TRUE:
						ZVAL_STRINGL(&new_val, "'t'", sizeof("'t'")-1);
						break;

					case IS_FALSE:
						ZVAL_STRINGL(&new_val, "'f'", sizeof("'f'")-1);
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects string, null, long or boolelan value for PostgreSQL '%s' (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			case PG_OID:
			case PG_INT2:
			case PG_INT4:
			case PG_INT8:
				switch (Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						}
						else {
							/* FIXME: better regex must be used */
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val), "^([+-]{0,1}[0-9]+)$", 0) == FAILURE) {
								err = 1;
							}
							else {
								ZVAL_STRINGL(&new_val, Z_STRVAL_P(val), Z_STRLEN_P(val));
							}
						}
						break;

					case IS_DOUBLE:
						ZVAL_DOUBLE(&new_val, Z_DVAL_P(val));
						convert_to_long_ex(&new_val);
						break;
						
					case IS_LONG:
						ZVAL_LONG(&new_val, Z_LVAL_P(val));
						break;
						
					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL, string, long or double value for pgsql '%s' (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			case PG_NUMERIC:
			case PG_MONEY:
			case PG_FLOAT4:
			case PG_FLOAT8:
				switch (Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						}
						else {
							/* better regex? */
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val), "^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$", 0) == FAILURE) {
								err = 1;
							}
							else {
								ZVAL_STRING(&new_val, Z_STRVAL_P(val));
							}
						}
						break;

					case IS_LONG:
						ZVAL_LONG(&new_val, Z_LVAL_P(val));
						break;

					case IS_DOUBLE:
						ZVAL_DOUBLE(&new_val, Z_DVAL_P(val));
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL, string, long or double value for PostgreSQL '%s' (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

				/* Exotic types are handled as string also.
				   Please feel free to add more valitions. Invalid query fails
				   at execution anyway. */
			case PG_TEXT:
			case PG_CHAR:
			case PG_VARCHAR:
				/* bit */
			case PG_BIT:
			case PG_VARBIT:
				/* geometric */
			case PG_LINE:
			case PG_LSEG:
			case PG_POINT:
			case PG_BOX:
			case PG_PATH:
			case PG_POLYGON:
			case PG_CIRCLE:
				/* unknown. JSON, Array etc */
			case PG_UNKNOWN:
				switch (Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							if (opt & PGSQL_CONV_FORCE_NULL) {
								ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
							} else {
								ZVAL_STRINGL(&new_val, "''", sizeof("''")-1);
							}
						}
						else {
							zend_string *str;
							/* PostgreSQL ignores \0 */
							str = zend_string_alloc(Z_STRLEN_P(val) * 2, 0);
							/* better to use PGSQLescapeLiteral since PGescapeStringConn does not handle special \ */
							str->len = PQescapeStringConn(pg_link, str->val, Z_STRVAL_P(val), Z_STRLEN_P(val), NULL);
							str = zend_string_realloc(str, str->len, 0);
							ZVAL_NEW_STR(&new_val, str);
							php_pgsql_add_quotes(&new_val, 1);
						}
						break;

					case IS_LONG:
						ZVAL_LONG(&new_val, Z_LVAL_P(val));
						convert_to_string_ex(&new_val);
						break;

					case IS_DOUBLE:
						ZVAL_DOUBLE(&new_val, Z_DVAL_P(val));
						convert_to_string_ex(&new_val);
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL, string, long or double value for PostgreSQL '%s' (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			case PG_UNIX_TIME:
			case PG_UNIX_TIME_INTERVAL:
				/* these are the actallay a integer */
				switch (Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						}
						else {
							/* better regex? */
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val), "^[0-9]+$", 0) == FAILURE) {
								err = 1;
							}
							else {
								ZVAL_STRINGL(&new_val, Z_STRVAL_P(val), Z_STRLEN_P(val));
								convert_to_long_ex(&new_val);
							}
						}
						break;

					case IS_DOUBLE:
						ZVAL_DOUBLE(&new_val, Z_DVAL_P(val));
						convert_to_long_ex(&new_val);
						break;

					case IS_LONG:
						ZVAL_LONG(&new_val, Z_LVAL_P(val));
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL, string, long or double value for '%s' (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			case PG_CIDR:
			case PG_INET:
				switch (Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						}
						else {
							/* better regex? IPV6 and IPV4 */
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val), "^(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]).){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]).){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))$", 0) == FAILURE) {
								err = 1;
							}
							else {
								ZVAL_STRINGL(&new_val, Z_STRVAL_P(val), Z_STRLEN_P(val));
								php_pgsql_add_quotes(&new_val, 1);
							}
						}
						break;
						
					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL or string for '%s' (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			case PG_TIME_WITH_TIMEZONE:
			case PG_TIMESTAMP:
			case PG_TIMESTAMP_WITH_TIMEZONE:
				switch(Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						} else if (!strcasecmp(Z_STRVAL_P(val), "now()")) {
							ZVAL_STRINGL(&new_val, "NOW()", sizeof("NOW()")-1);
						} else {
							/* better regex? */
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val), "^([0-9]{4}[/-][0-9]{1,2}[/-][0-9]{1,2})([ \\t]+(([0-9]{1,2}:[0-9]{1,2}){1}(:[0-9]{1,2}){0,1}(\\.[0-9]+){0,1}([ \\t]*([+-][0-9]{1,4}(:[0-9]{1,2}){0,1}|[-a-zA-Z_/+]{1,50})){0,1})){0,1}$", 1) == FAILURE) {
								err = 1;
							} else {
								ZVAL_STRING(&new_val, Z_STRVAL_P(val));
								php_pgsql_add_quotes(&new_val, 1);
							}
						}
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL or string for PostgreSQL %s field (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			case PG_DATE:
				switch(Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						}
						else {
							/* FIXME: better regex must be used */
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val), "^([0-9]{4}[/-][0-9]{1,2}[/-][0-9]{1,2})$", 1) == FAILURE) {
								err = 1;
							}
							else {
								ZVAL_STRINGL(&new_val, Z_STRVAL_P(val), Z_STRLEN_P(val));
								php_pgsql_add_quotes(&new_val, 1);
							}
						}
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL or string for PostgreSQL %s field (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			case PG_TIME:
				switch(Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						}
						else {
							/* FIXME: better regex must be used */
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val), "^(([0-9]{1,2}:[0-9]{1,2}){1}(:[0-9]{1,2}){0,1})){0,1}$", 1) == FAILURE) {
								err = 1;
							}
							else {
								ZVAL_STRINGL(&new_val, Z_STRVAL_P(val), Z_STRLEN_P(val));
								php_pgsql_add_quotes(&new_val, 1);
							}
						}
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL or string for PostgreSQL %s field (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			case PG_INTERVAL:
				switch(Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRING(&new_val, "NULL");
						}
						else {

							/* From the Postgres docs:

							   interval values can be written with the following syntax:
							   [@] quantity unit [quantity unit...] [direction]
							   
							   Where: quantity is a number (possibly signed); unit is second, minute, hour,
							   day, week, month, year, decade, century, millennium, or abbreviations or
							   plurals of these units [note not *all* abbreviations] ; direction can be
							   ago or empty. The at sign (@) is optional noise.
							   
							   ...
							   
							   Quantities of days, hours, minutes, and seconds can be specified without explicit
							   unit markings. For example, '1 12:59:10' is read the same as '1 day 12 hours 59 min 10
							   sec'.
							*/
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val),
														"^(@?[ \\t]+)?("

														/* Textual time units and their abbreviations: */
														"(([-+]?[ \\t]+)?"
														"[0-9]+(\\.[0-9]*)?[ \\t]*"
														"(millenniums|millennia|millennium|mil|mils|"
														"centuries|century|cent|c|"
														"decades|decade|dec|decs|"
														"years|year|y|"
														"months|month|mon|"
														"weeks|week|w|" 
														"days|day|d|"
														"hours|hour|hr|hrs|h|"
														"minutes|minute|mins|min|m|"
														"seconds|second|secs|sec|s))+|"

														/* Textual time units plus (dd)* hh[:mm[:ss]] */
														"((([-+]?[ \\t]+)?"
														"[0-9]+(\\.[0-9]*)?[ \\t]*"
														"(millenniums|millennia|millennium|mil|mils|"
														"centuries|century|cent|c|"
														"decades|decade|dec|decs|"
														"years|year|y|"
														"months|month|mon|"
														"weeks|week|w|"
														"days|day|d))+" 
														"([-+]?[ \\t]+"
														"([0-9]+[ \\t]+)+"				 /* dd */
														"(([0-9]{1,2}:){0,2}[0-9]{0,2})" /* hh:[mm:[ss]] */
														")?))"
														"([ \\t]+ago)?$",
														1) == FAILURE) {
								err = 1;
							}
							else {
								ZVAL_STRING(&new_val, Z_STRVAL_P(val));
								php_pgsql_add_quotes(&new_val, 1);
							}
						}
						break;

					case IS_NULL:
						ZVAL_STRING(&new_val, "NULL");
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL or string for PostgreSQL %s field (%s)", Z_STRVAL_P(type), field->val);
				}
				break;
#ifdef HAVE_PQESCAPE
			case PG_BYTEA:
				switch (Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRING(&new_val, "NULL");
						}
						else {
							unsigned char *tmp;
							size_t to_len;
							smart_str s = {0};
#ifdef HAVE_PQESCAPE_BYTEA_CONN
							tmp = PQescapeByteaConn(pg_link, (unsigned char *)Z_STRVAL_P(val), Z_STRLEN_P(val), &to_len);
#else
							tmp = PQescapeBytea(Z_STRVAL_P(val), (unsigned char *)Z_STRLEN_P(val), &to_len);
#endif
							ZVAL_STRINGL(&new_val, (char *)tmp, to_len - 1); /* PQescapeBytea's to_len includes additional '\0' */
							PQfreemem(tmp);
							php_pgsql_add_quotes(&new_val, 1);
							smart_str_appendl(&s, Z_STRVAL(new_val), Z_STRLEN(new_val));
							smart_str_0(&s);
							zval_ptr_dtor(&new_val);
							ZVAL_NEW_STR(&new_val, s.s);
						}
						break;

					case IS_LONG:
						ZVAL_LONG(&new_val, Z_LVAL_P(val));
						convert_to_string_ex(&new_val);
						break;

					case IS_DOUBLE:
						ZVAL_DOUBLE(&new_val, Z_DVAL_P(val));
						convert_to_string_ex(&new_val);
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL, string, long or double value for PostgreSQL '%s' (%s)", Z_STRVAL_P(type), field->val);
				}
				break;
				
#endif
			case PG_MACADDR:
				switch(Z_TYPE_P(val)) {
					case IS_STRING:
						if (Z_STRLEN_P(val) == 0) {
							ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						}
						else {
							if (php_pgsql_convert_match(Z_STRVAL_P(val), Z_STRLEN_P(val), "^([0-9a-f]{2,2}:){5,5}[0-9a-f]{2,2}$", 1) == FAILURE) {
								err = 1;
							}
							else {
								ZVAL_STRINGL(&new_val, Z_STRVAL_P(val), Z_STRLEN_P(val));
								php_pgsql_add_quotes(&new_val, 1);
							}
						}
						break;

					case IS_NULL:
						ZVAL_STRINGL(&new_val, "NULL", sizeof("NULL")-1);
						break;

					default:
						err = 1;
				}
				PGSQL_CONV_CHECK_IGNORE();
				if (err) {
					php_error_docref(NULL, E_NOTICE, "Expects NULL or string for PostgreSQL %s field (%s)", Z_STRVAL_P(type), field->val);
				}
				break;

			default:
				/* should not happen */
				php_error_docref(NULL, E_NOTICE, "Unknown or system data type '%s' for '%s'. Report error", Z_STRVAL_P(type), field->val);
				err = 1;
				break;
		} /* switch */

		if (err) {
			zval_ptr_dtor(&new_val);
			break; /* break out for() */
		}
		/* If field is NULL and HAS DEFAULT, should be skipped */
		if (!skip_field) {
			char *escaped;

			if (_php_pgsql_detect_identifier_escape(field->val, field->len) == SUCCESS) {
				zend_hash_update(Z_ARRVAL_P(result), field, &new_val);
			} else {
				escaped = PGSQLescapeIdentifier(pg_link, field->val, field->len);
				add_assoc_zval(result, escaped, &new_val);
				PGSQLfree(escaped);
			}
		}
	} ZEND_HASH_FOREACH_END(); /* for */

	zval_ptr_dtor(&meta);

	if (err) {
		/* shouldn't destroy & free zval here */
		return FAILURE;
	}
	return SUCCESS;
}
