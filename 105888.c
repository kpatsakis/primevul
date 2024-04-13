PHP_FUNCTION(pg_lo_read)
{
	zval *pgsql_id;
	long len;
	int buf_len = PGSQL_LO_READ_BUF_SIZE, nbytes, argc = ZEND_NUM_ARGS();
	char *buf;
	pgLofp *pgsql;

	if (zend_parse_parameters(argc TSRMLS_CC, "r|l", &pgsql_id, &len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, &pgsql_id, -1, "PostgreSQL large object", le_lofp);

	if (argc > 1) {
		buf_len = len;
	}
	
	buf = (char *) safe_emalloc(sizeof(char), (buf_len+1), 0);
	if ((nbytes = lo_read((PGconn *)pgsql->conn, pgsql->lofd, buf, buf_len))<0) {
		efree(buf);
		RETURN_FALSE;
	}

	buf[nbytes] = '\0';
	RETURN_STRINGL(buf, nbytes, 0);
}
