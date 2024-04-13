PHP_FUNCTION(sleep)
{
	long num;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &num) == FAILURE) {
		RETURN_FALSE;
	}
	if (num < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Number of seconds must be greater than or equal to 0");
		RETURN_FALSE;
	}
#ifdef PHP_SLEEP_NON_VOID
	RETURN_LONG(php_sleep(num));
#else
	php_sleep(num);
#endif

}
