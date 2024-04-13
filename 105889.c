PHP_FUNCTION(pg_lo_write)
{
  	zval *pgsql_id;
  	char *str;
  	long z_len;
	int str_len, nbytes;
	int len;
	pgLofp *pgsql;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "rs|l", &pgsql_id, &str, &str_len, &z_len) == FAILURE) {
		return;
	}

	if (argc > 2) {
		if (z_len > str_len) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot write more than buffer size %d. Tried to write %ld", str_len, z_len);
			RETURN_FALSE;
		}
		if (z_len < 0) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Buffer size must be larger than 0, but %ld was specified", z_len);
			RETURN_FALSE;
		}
		len = z_len;
	}
	else {
		len = str_len;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, &pgsql_id, -1, "PostgreSQL large object", le_lofp);

	if ((nbytes = lo_write((PGconn *)pgsql->conn, pgsql->lofd, str, len)) == -1) {
		RETURN_FALSE;
	}

	RETURN_LONG(nbytes);
}
