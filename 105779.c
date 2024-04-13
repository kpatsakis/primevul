static int mysqlnd_stream_array_to_fd_set(MYSQLND ** conn_array, fd_set * fds, php_socket_t * max_fd TSRMLS_DC)
{
	php_socket_t this_fd;
	php_stream *stream = NULL;
	int cnt = 0;
	MYSQLND **p = conn_array;

	while (*p) {
		/* get the fd.
		 * NB: Most other code will NOT use the PHP_STREAM_CAST_INTERNAL flag
		 * when casting.  It is only used here so that the buffered data warning
		 * is not displayed.
		 * */
		stream = (*p)->data->net->stream;
		if (stream != NULL && SUCCESS == php_stream_cast(stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL,
										(void*)&this_fd, 1) && this_fd >= 0) {

			PHP_SAFE_FD_SET(this_fd, fds);

			if (this_fd > *max_fd) {
				*max_fd = this_fd;
			}
			cnt++;
		}
		p++;
	}
	return cnt ? 1 : 0;
}
