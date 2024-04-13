PHP_FUNCTION(parse_ini_file)
{
	char *filename = NULL;
	int filename_len = 0;
	zend_bool process_sections = 0;
	long scanner_mode = ZEND_INI_SCANNER_NORMAL;
	zend_file_handle fh;
	zend_ini_parser_cb_t ini_parser_cb;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p|bl", &filename, &filename_len, &process_sections, &scanner_mode) == FAILURE) {
		RETURN_FALSE;
	}

	if (filename_len == 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Filename cannot be empty!");
		RETURN_FALSE;
	}

	/* Set callback function */
	if (process_sections) {
		BG(active_ini_file_section) = NULL;
		ini_parser_cb = (zend_ini_parser_cb_t) php_ini_parser_cb_with_sections;
	} else {
		ini_parser_cb = (zend_ini_parser_cb_t) php_simple_ini_parser_cb;
	}

	/* Setup filehandle */
	memset(&fh, 0, sizeof(fh));
	fh.filename = filename;
	fh.type = ZEND_HANDLE_FILENAME;

	array_init(return_value);
	if (zend_parse_ini_file(&fh, 0, scanner_mode, ini_parser_cb, return_value TSRMLS_CC) == FAILURE) {
		zend_hash_destroy(Z_ARRVAL_P(return_value));
		efree(Z_ARRVAL_P(return_value));
		RETURN_FALSE;
	}
}
