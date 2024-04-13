PHP_FUNCTION(pg_lo_read)
{
	zval *pgsql_id;
	zend_long len;
	size_t buf_len = PGSQL_LO_READ_BUF_SIZE;
	int nbytes, argc = ZEND_NUM_ARGS();
	zend_string *buf;
	pgLofp *pgsql;

	if (zend_parse_parameters(argc, "r|l", &pgsql_id, &len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_id, -1, "PostgreSQL large object", le_lofp);

	if (argc > 1) {
		buf_len = len < 0 ? 0 : len;
	}
	
	buf = zend_string_alloc(buf_len, 0);
	if ((nbytes = lo_read((PGconn *)pgsql->conn, pgsql->lofd, buf->val, buf->len))<0) {
		zend_string_free(buf);
		RETURN_FALSE;
	}

	buf->len = nbytes;
	buf->val[buf->len] = '\0';
	RETURN_STR(buf);
}
