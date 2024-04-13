_mysqlnd_poll(MYSQLND **r_array, MYSQLND **e_array, MYSQLND ***dont_poll, long sec, long usec, int * desc_num TSRMLS_DC)
{
	struct timeval	tv;
	struct timeval *tv_p = NULL;
	fd_set			rfds, wfds, efds;
	php_socket_t	max_fd = 0;
	int				retval, sets = 0;
	int				set_count, max_set_count = 0;

	DBG_ENTER("_mysqlnd_poll");
	if (sec < 0 || usec < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Negative values passed for sec and/or usec");
		DBG_RETURN(FAIL);
	}

	*dont_poll = mysqlnd_stream_array_check_for_readiness(r_array TSRMLS_CC);

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	if (r_array != NULL) {
		set_count = mysqlnd_stream_array_to_fd_set(r_array, &rfds, &max_fd TSRMLS_CC);
		if (set_count > max_set_count) {
			max_set_count = set_count;
		}
		sets += set_count;
	}

	if (e_array != NULL) {
		set_count = mysqlnd_stream_array_to_fd_set(e_array, &efds, &max_fd TSRMLS_CC);
		if (set_count > max_set_count) {
			max_set_count = set_count;
		}
		sets += set_count;
	}

	if (!sets) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, *dont_poll ? "All arrays passed are clear":"No stream arrays were passed");
		DBG_ERR_FMT(*dont_poll ? "All arrays passed are clear":"No stream arrays were passed");
		DBG_RETURN(FAIL);
	}

	PHP_SAFE_MAX_FD(max_fd, max_set_count);

	/* Solaris + BSD do not like microsecond values which are >= 1 sec */
	if (usec > 999999) {
		tv.tv_sec = sec + (usec / 1000000);
		tv.tv_usec = usec % 1000000;
	} else {
		tv.tv_sec = sec;
		tv.tv_usec = usec;
	}

	tv_p = &tv;

	retval = php_select(max_fd + 1, &rfds, &wfds, &efds, tv_p);

	if (retval == -1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "unable to select [%d]: %s (max_fd=%d)",
						errno, strerror(errno), max_fd);
		DBG_RETURN(FAIL);
	}

	if (r_array != NULL) {
		mysqlnd_stream_array_from_fd_set(r_array, &rfds TSRMLS_CC);
	}
	if (e_array != NULL) {
		mysqlnd_stream_array_from_fd_set(e_array, &efds TSRMLS_CC);
	}

	*desc_num = retval;
	DBG_RETURN(PASS);
}
