PHP_FUNCTION(pg_lo_tell)
{
	zval *pgsql_id = NULL;
	int offset = 0;
	pgLofp *pgsql;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &pgsql_id) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, &pgsql_id, -1, "PostgreSQL large object", le_lofp);

	offset = lo_tell((PGconn *)pgsql->conn, pgsql->lofd);
	RETURN_LONG(offset);
}
