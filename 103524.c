PHP_FUNCTION(curl_strerror)
{
	zend_long code;
	const char *str;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &code) == FAILURE) {
		return;
	}

	str = curl_easy_strerror(code);
	if (str) {
		RETURN_STRING(str);
	} else {
		RETURN_NULL();
	}
}
