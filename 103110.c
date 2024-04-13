PHP_FUNCTION(call_user_method)
{
	zval ***params = NULL;
	int n_params = 0;
	zval *retval_ptr;
	zval *callback, *object;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/z*", &callback, &object, &params, &n_params) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(object) != IS_OBJECT &&
		Z_TYPE_P(object) != IS_STRING
	) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Second argument is not an object or class name");
		if (params) {
			efree(params);
		}
		RETURN_FALSE;
	}

	convert_to_string(callback);

	if (call_user_function_ex(EG(function_table), &object, callback, &retval_ptr, n_params, params, 0, NULL TSRMLS_CC) == SUCCESS) {
		if (retval_ptr) {
			COPY_PZVAL_TO_ZVAL(*return_value, retval_ptr);
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call %s()", Z_STRVAL_P(callback));
	}
	if (n_params) {
		efree(params);
	}
}
