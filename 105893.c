PHP_FUNCTION(pg_lo_seek)
{
	zval *pgsql_id = NULL;
	long offset = 0, whence = SEEK_CUR;
	pgLofp *pgsql;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "rl|l", &pgsql_id, &offset, &whence) == FAILURE) {
		return;
	}
	if (whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid whence parameter");
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, &pgsql_id, -1, "PostgreSQL large object", le_lofp);

	if (lo_lseek((PGconn *)pgsql->conn, pgsql->lofd, offset, whence) > -1) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
