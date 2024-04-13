PHP_FUNCTION(usleep)
{
#if HAVE_USLEEP
	long num;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &num) == FAILURE) {
		return;
	}
	if (num < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Number of microseconds must be greater than or equal to 0");
		RETURN_FALSE;
	}
	usleep(num);
#endif
}
