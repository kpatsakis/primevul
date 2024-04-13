PHP_FUNCTION(forward_static_call_array)
{
	zval *params, *retval_ptr = NULL;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "fa/", &fci, &fci_cache, &params) == FAILURE) {
		return;
	}

	zend_fcall_info_args(&fci, params TSRMLS_CC);
	fci.retval_ptr_ptr = &retval_ptr;

	if (EG(called_scope) &&
		instanceof_function(EG(called_scope), fci_cache.calling_scope TSRMLS_CC)) {
			fci_cache.called_scope = EG(called_scope);
	}

	if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
		COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
	}

	zend_fcall_info_args_clear(&fci, 1);
}
