static int php_pgsql_fd_cast(php_stream *stream, int cast_as, void **ret) /* {{{ */
{
	PGconn *pgsql = (PGconn *) stream->abstract;
	int fd_number;

	switch (cast_as)	{
		case PHP_STREAM_AS_FD_FOR_SELECT:
		case PHP_STREAM_AS_FD:
		case PHP_STREAM_AS_SOCKETD:
			if (ret) {
				fd_number = PQsocket(pgsql);
				if (fd_number == -1) {
					return FAILURE;
				}

				*(php_socket_t *)ret = fd_number;
				return SUCCESS;
			}
		default:
			return FAILURE;
	}
}
/* }}} */
