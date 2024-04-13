PHP_FUNCTION(error_log)
{
	char *message, *opt = NULL, *headers = NULL;
	int message_len, opt_len = 0, headers_len = 0;
	int opt_err = 0, argc = ZEND_NUM_ARGS();
	long erropt = 0;

	if (zend_parse_parameters(argc TSRMLS_CC, "s|lps", &message, &message_len, &erropt, &opt, &opt_len, &headers, &headers_len) == FAILURE) {
		return;
	}

	if (argc > 1) {
		opt_err = erropt;
	}

	if (_php_error_log_ex(opt_err, message, message_len, opt, headers TSRMLS_CC) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
