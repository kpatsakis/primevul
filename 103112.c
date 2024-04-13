PHP_FUNCTION(forward_static_call)
{
	zval *retval_ptr = NULL;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f*", &fci, &fci_cache, &fci.params, &fci.param_count) == FAILURE) {
		return;
	}

	if (!EG(active_op_array)->scope) {
		zend_error(E_ERROR, "Cannot call forward_static_call() when no class scope is active");
	}

	fci.retval_ptr_ptr = &retval_ptr;

	if (EG(called_scope) &&
		instanceof_function(EG(called_scope), fci_cache.calling_scope TSRMLS_CC)) {
			fci_cache.called_scope = EG(called_scope);
	}
	
	if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
		COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
	}

	if (fci.params) {
		efree(fci.params);
	}
}
