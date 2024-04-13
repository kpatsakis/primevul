PHP_FUNCTION(ini_get)
{
	char *varname, *str;
	int varname_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &varname, &varname_len) == FAILURE) {
		return;
	}

	str = zend_ini_string(varname, varname_len + 1, 0);

	if (!str) {
		RETURN_FALSE;
	}

	RETURN_STRING(str, 1);
}
