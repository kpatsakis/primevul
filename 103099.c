PHP_FUNCTION(time_nanosleep)
{
	long tv_sec, tv_nsec;
	struct timespec php_req, php_rem;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &tv_sec, &tv_nsec) == FAILURE) {
		return;
	}

	if (tv_sec < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "The seconds value must be greater than 0");
		RETURN_FALSE;
	}
	if (tv_nsec < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "The nanoseconds value must be greater than 0");
		RETURN_FALSE;
	}

	php_req.tv_sec = (time_t) tv_sec;
	php_req.tv_nsec = tv_nsec;
	if (!nanosleep(&php_req, &php_rem)) {
		RETURN_TRUE;
	} else if (errno == EINTR) {
		array_init(return_value);
		add_assoc_long_ex(return_value, "seconds", sizeof("seconds"), php_rem.tv_sec);
		add_assoc_long_ex(return_value, "nanoseconds", sizeof("nanoseconds"), php_rem.tv_nsec);
		return;
	} else if (errno == EINVAL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "nanoseconds was not in the range 0 to 999 999 999 or seconds was negative");
	}

	RETURN_FALSE;
}
