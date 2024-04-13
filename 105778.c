static int mysqlnd_stream_array_from_fd_set(MYSQLND ** conn_array, fd_set * fds TSRMLS_DC)
{
	php_socket_t this_fd;
	php_stream *stream = NULL;
	int ret = 0;
	zend_bool disproportion = FALSE;


	MYSQLND **fwd = conn_array, **bckwd = conn_array;

	while (*fwd) {
		stream = (*fwd)->data->net->stream;
		if (stream != NULL && SUCCESS == php_stream_cast(stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL,
										(void*)&this_fd, 1) && this_fd >= 0) {
			if (PHP_SAFE_FD_ISSET(this_fd, fds)) {
				if (disproportion) {
					*bckwd = *fwd;
				}
				bckwd++;
				fwd++;
				ret++;
				continue;
			}
		}
		disproportion = TRUE;
		fwd++;
	}
	*bckwd = NULL;/* NULL-terminate the list */

	return ret;
}
