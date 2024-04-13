PHP_FUNCTION(highlight_file)
{
	char *filename;
	int filename_len, ret;
	zend_syntax_highlighter_ini syntax_highlighter_ini;
	zend_bool i = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p|b", &filename, &filename_len, &i) == FAILURE) {
		RETURN_FALSE;
	}

	if (php_check_open_basedir(filename TSRMLS_CC)) {
		RETURN_FALSE;
	}

	if (i) {
		php_output_start_default(TSRMLS_C);
	}

	php_get_highlight_struct(&syntax_highlighter_ini);

	ret = highlight_file(filename, &syntax_highlighter_ini TSRMLS_CC);

	if (ret == FAILURE) {
		if (i) {
			php_output_end(TSRMLS_C);
		}
		RETURN_FALSE;
	}

	if (i) {
		php_output_get_contents(return_value TSRMLS_CC);
		php_output_discard(TSRMLS_C);
	} else {
		RETURN_TRUE;
	}
}
