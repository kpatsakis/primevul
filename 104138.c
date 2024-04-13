PHP_FUNCTION(apache_response_headers) /* {{{ */
{
	if (ZEND_NUM_ARGS() > 0) {
		WRONG_PARAM_COUNT;
	}

	if (!&SG(sapi_headers).headers) {
		RETURN_FALSE;
	}
	array_init(return_value);
	zend_llist_apply_with_argument(&SG(sapi_headers).headers, (llist_apply_with_arg_func_t)add_response_header, return_value TSRMLS_CC);
}
/* }}} */
