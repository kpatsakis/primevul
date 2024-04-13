static void php_pgsql_escape_internal(INTERNAL_FUNCTION_PARAMETERS, int escape_literal) /* {{{ */ {
	char *from = NULL;
	zval *pgsql_link = NULL;
	PGconn *pgsql;
	size_t from_len;
	int id = -1;
	char *tmp;

	switch (ZEND_NUM_ARGS()) {
		case 1:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &from, &from_len) == FAILURE) {
				return;
			}
			pgsql_link = NULL;
			id = FETCH_DEFAULT_LINK();
			break;

		default:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "rs", &pgsql_link, &from, &from_len) == FAILURE) {
				return;
			}
			break;
	}

	if (pgsql_link == NULL && id == -1) {
		php_error_docref(NULL, E_WARNING,"Cannot get default pgsql link");
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	if (pgsql == NULL) {
		php_error_docref(NULL, E_WARNING,"Cannot get pgsql link");
		RETURN_FALSE;
	}

	if (escape_literal) {
		tmp = PGSQLescapeLiteral(pgsql, from, (size_t)from_len);
	} else {
		tmp = PGSQLescapeIdentifier(pgsql, from, (size_t)from_len);
	}
	if (!tmp) {
		php_error_docref(NULL, E_WARNING,"Failed to escape");
		RETURN_FALSE;
	}

	RETVAL_STRING(tmp);
	PGSQLfree(tmp);
}
/* }}} */
