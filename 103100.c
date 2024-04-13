PHP_FUNCTION(time_sleep_until)
{
	double d_ts, c_ts;
	struct timeval tm;
	struct timespec php_req, php_rem;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &d_ts) == FAILURE) {
		return;
	}

	if (gettimeofday((struct timeval *) &tm, NULL) != 0) {
		RETURN_FALSE;
	}

	c_ts = (double)(d_ts - tm.tv_sec - tm.tv_usec / 1000000.00);
	if (c_ts < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Sleep until to time is less than current time");
		RETURN_FALSE;
	}

	php_req.tv_sec = (time_t) c_ts;
	if (php_req.tv_sec > c_ts) { /* rounding up occurred */
		php_req.tv_sec--;
	}
	/* 1sec = 1000000000 nanoseconds */
	php_req.tv_nsec = (long) ((c_ts - php_req.tv_sec) * 1000000000.00);

	while (nanosleep(&php_req, &php_rem)) {
		if (errno == EINTR) {
			php_req.tv_sec = php_rem.tv_sec;
			php_req.tv_nsec = php_rem.tv_nsec;
		} else {
			RETURN_FALSE;
		}
	}

	RETURN_TRUE;
}
