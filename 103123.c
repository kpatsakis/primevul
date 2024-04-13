PHP_FUNCTION(get_include_path)
{
	char *str;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}

	str = zend_ini_string("include_path", sizeof("include_path"), 0);

	if (str == NULL) {
		RETURN_FALSE;
	}

	RETURN_STRING(str, 1);
}
