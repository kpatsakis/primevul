PHP_FUNCTION(parse_ini_string)
{
	char *string = NULL, *str = NULL;
	int str_len = 0;
	zend_bool process_sections = 0;
	long scanner_mode = ZEND_INI_SCANNER_NORMAL;
	zend_ini_parser_cb_t ini_parser_cb;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|bl", &str, &str_len, &process_sections, &scanner_mode) == FAILURE) {
		RETURN_FALSE;
	}

	if (INT_MAX - str_len < ZEND_MMAP_AHEAD) {
		RETVAL_FALSE;
	}

	/* Set callback function */
	if (process_sections) {
		BG(active_ini_file_section) = NULL;
		ini_parser_cb = (zend_ini_parser_cb_t) php_ini_parser_cb_with_sections;
	} else {
		ini_parser_cb = (zend_ini_parser_cb_t) php_simple_ini_parser_cb;
	}

	/* Setup string */
	string = (char *) emalloc(str_len + ZEND_MMAP_AHEAD);
	memcpy(string, str, str_len);
	memset(string + str_len, 0, ZEND_MMAP_AHEAD);

	array_init(return_value);
	if (zend_parse_ini_string(string, 0, scanner_mode, ini_parser_cb, return_value TSRMLS_CC) == FAILURE) {
		zend_hash_destroy(Z_ARRVAL_P(return_value));
		efree(Z_ARRVAL_P(return_value));
		RETVAL_FALSE;
	}
	efree(string);
}
