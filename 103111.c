PHP_FUNCTION(call_user_method_array)
{
	zval *params, ***method_args = NULL, *retval_ptr;
	zval *callback, *object;
	HashTable *params_ar;
	int num_elems, element = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/zA/", &callback, &object, &params) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(object) != IS_OBJECT &&
		Z_TYPE_P(object) != IS_STRING
	) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Second argument is not an object or class name");
		RETURN_FALSE;
	}

	convert_to_string(callback);

	params_ar = HASH_OF(params);
	num_elems = zend_hash_num_elements(params_ar);
	method_args = (zval ***) safe_emalloc(sizeof(zval **), num_elems, 0);

	for (zend_hash_internal_pointer_reset(params_ar);
		zend_hash_get_current_data(params_ar, (void **) &(method_args[element])) == SUCCESS;
		zend_hash_move_forward(params_ar)
	) {
		element++;
	}

	if (call_user_function_ex(EG(function_table), &object, callback, &retval_ptr, num_elems, method_args, 0, NULL TSRMLS_CC) == SUCCESS) {
		if (retval_ptr) {
			COPY_PZVAL_TO_ZVAL(*return_value, retval_ptr);
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call %s()", Z_STRVAL_P(callback));
	}

	efree(method_args);
}
