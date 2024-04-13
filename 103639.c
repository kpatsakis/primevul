PHP_FUNCTION(pg_socket)
{
	zval *pgsql_link;
	php_stream *stream;
	PGconn *pgsql;
	int id = -1;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &pgsql_link) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	stream = php_stream_alloc(&php_stream_pgsql_fd_ops, pgsql, NULL, "r");

	if (stream) {
		php_stream_to_zval(stream, return_value);
		return;
	}

	RETURN_FALSE;
}
