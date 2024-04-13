PHP_FUNCTION(pg_escape_bytea)
{
	char *from = NULL, *to = NULL;
	size_t to_len;
	size_t from_len;
	int id = -1;
#ifdef HAVE_PQESCAPE_BYTEA_CONN
	PGconn *pgsql;
#endif
	zval *pgsql_link;

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

#ifdef HAVE_PQESCAPE_BYTEA_CONN
	if (pgsql_link != NULL || id != -1) {
		ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
		to = (char *)PQescapeByteaConn(pgsql, (unsigned char *)from, (size_t)from_len, &to_len);
	} else
#endif
		to = (char *)PQescapeBytea((unsigned char*)from, from_len, &to_len);

	RETVAL_STRINGL(to, to_len-1); /* to_len includes additional '\0' */
}
