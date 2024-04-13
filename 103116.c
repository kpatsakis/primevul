PHP_FUNCTION(php_strip_whitespace)
{
	char *filename;
	int filename_len;
	zend_lex_state original_lex_state;
	zend_file_handle file_handle = {0};

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p", &filename, &filename_len) == FAILURE) {
		RETURN_FALSE;
	}

	php_output_start_default(TSRMLS_C);

	file_handle.type = ZEND_HANDLE_FILENAME;
	file_handle.filename = filename;
	file_handle.free_filename = 0;
	file_handle.opened_path = NULL;
	zend_save_lexical_state(&original_lex_state TSRMLS_CC);
	if (open_file_for_scanning(&file_handle TSRMLS_CC) == FAILURE) {
		zend_restore_lexical_state(&original_lex_state TSRMLS_CC);
		php_output_end(TSRMLS_C);
		RETURN_EMPTY_STRING();
	}

	zend_strip(TSRMLS_C);

	zend_destroy_file_handle(&file_handle TSRMLS_CC);
	zend_restore_lexical_state(&original_lex_state TSRMLS_CC);

	php_output_get_contents(return_value TSRMLS_CC);
	php_output_discard(TSRMLS_C);
}
