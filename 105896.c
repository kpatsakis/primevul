PHP_FUNCTION(pg_set_client_encoding)
{
	char *encoding;
	int encoding_len;
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;

	if (argc == 1) {
		if (zend_parse_parameters(argc TSRMLS_CC, "s", &encoding, &encoding_len) == FAILURE) {
			return;
		}
		id = PGG(default_link);
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc TSRMLS_CC, "rs", &pgsql_link, &encoding, &encoding_len) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	Z_LVAL_P(return_value) = PQsetClientEncoding(pgsql, encoding);
	Z_TYPE_P(return_value) = IS_LONG;
}
