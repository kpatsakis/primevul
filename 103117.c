PHP_FUNCTION(highlight_string)
{
	zval **expr;
	zend_syntax_highlighter_ini syntax_highlighter_ini;
	char *hicompiled_string_description;
	zend_bool i = 0;
	int old_error_reporting = EG(error_reporting);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Z|b", &expr, &i) == FAILURE) {
		RETURN_FALSE;
	}
	convert_to_string_ex(expr);

	if (i) {
		php_output_start_default(TSRMLS_C);
	}

	EG(error_reporting) = E_ERROR;

	php_get_highlight_struct(&syntax_highlighter_ini);

	hicompiled_string_description = zend_make_compiled_string_description("highlighted code" TSRMLS_CC);

	if (highlight_string(*expr, &syntax_highlighter_ini, hicompiled_string_description TSRMLS_CC) == FAILURE) {
		efree(hicompiled_string_description);
		EG(error_reporting) = old_error_reporting;
		if (i) {
			php_output_end(TSRMLS_C);
		}
		RETURN_FALSE;
	}
	efree(hicompiled_string_description);

	EG(error_reporting) = old_error_reporting;

	if (i) {
		php_output_get_contents(return_value TSRMLS_CC);
		php_output_discard(TSRMLS_C);
	} else {
		RETURN_TRUE;
	}
}
