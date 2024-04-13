PHP_FUNCTION(pg_escape_string)
{
	char *from = NULL, *to = NULL;
	zval *pgsql_link;
#ifdef HAVE_PQESCAPE_CONN
	PGconn *pgsql;
#endif
	int to_len;
	int from_len;
	int id = -1;

	switch (ZEND_NUM_ARGS()) {
		case 1:
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &from, &from_len) == FAILURE) {
				return;
			}
			pgsql_link = NULL;
			id = PGG(default_link);
			break;

		default:
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &pgsql_link, &from, &from_len) == FAILURE) {
				return;
			}
			break;
	}

	to = (char *) safe_emalloc(from_len, 2, 1);
#ifdef HAVE_PQESCAPE_CONN
	if (pgsql_link != NULL || id != -1) {
		ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);
		to_len = (int) PQescapeStringConn(pgsql, to, from, (size_t)from_len, NULL);
	} else
#endif
		to_len = (int) PQescapeString(to, from, (size_t)from_len);

	RETURN_STRINGL(to, to_len, 0);
}
