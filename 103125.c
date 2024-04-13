PHP_FUNCTION(print_r)
{
	zval *var;
	zend_bool do_return = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|b", &var, &do_return) == FAILURE) {
		RETURN_FALSE;
	}

	if (do_return) {
		php_output_start_default(TSRMLS_C);
	}

	zend_print_zval_r(var, 0 TSRMLS_CC);

	if (do_return) {
		php_output_get_contents(return_value TSRMLS_CC);
		php_output_discard(TSRMLS_C);
	} else {
		RETURN_TRUE;
	}
}
