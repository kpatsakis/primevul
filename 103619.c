PHP_FUNCTION(pg_escape_string)
{
	zend_string *from = NULL, *to = NULL;
	zval *pgsql_link;
#ifdef HAVE_PQESCAPE_CONN
	PGconn *pgsql;
#endif
	int id = -1;

	switch (ZEND_NUM_ARGS()) {
		case 1:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &from) == FAILURE) {
				return;
			}
			pgsql_link = NULL;
			id = FETCH_DEFAULT_LINK();
			break;

		default:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "rS", &pgsql_link, &from) == FAILURE) {
				return;
			}
			break;
	}

	to = zend_string_alloc(from->len * 2, 0);
#ifdef HAVE_PQESCAPE_CONN
	if (pgsql_link != NULL || id != -1) {
		ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
		to->len = PQescapeStringConn(pgsql, to->val, from->val, from->len, NULL);
	} else
#endif
	{
		to->len = PQescapeString(to->val, from->val, from->len);
	}

	to = zend_string_realloc(to, to->len, 0);
	RETURN_STR(to);
}
