PHP_FUNCTION(pg_lo_write)
{
  	zval *pgsql_id;
  	char *str;
  	zend_long z_len;
	size_t str_len, nbytes;
	size_t len;
	pgLofp *pgsql;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "rs|l", &pgsql_id, &str, &str_len, &z_len) == FAILURE) {
		return;
	}

	if (argc > 2) {
		if (z_len > (zend_long)str_len) {
			php_error_docref(NULL, E_WARNING, "Cannot write more than buffer size %d. Tried to write %pd", str_len, z_len);
			RETURN_FALSE;
		}
		if (z_len < 0) {
			php_error_docref(NULL, E_WARNING, "Buffer size must be larger than 0, but %pd was specified", z_len);
			RETURN_FALSE;
		}
		len = z_len;
	}
	else {
		len = str_len;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_id, -1, "PostgreSQL large object", le_lofp);

	if ((nbytes = lo_write((PGconn *)pgsql->conn, pgsql->lofd, str, len)) == -1) {
		RETURN_FALSE;
	}

	RETURN_LONG(nbytes);
}
