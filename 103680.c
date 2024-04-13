static int php_pgsql_fd_set_option(php_stream *stream, int option, int value, void *ptrparam) /* {{{ */
{
	PGconn *pgsql = (PGconn *) stream->abstract;
	switch (option) {
		case PHP_STREAM_OPTION_BLOCKING:
			return PQ_SETNONBLOCKING(pgsql, value);
		default:
			return FAILURE;
	}
}
/* }}} */
