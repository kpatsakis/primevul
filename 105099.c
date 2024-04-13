static zend_bool php_check_cancel_upload(php_session_rfc1867_progress *progress TSRMLS_DC) /* {{{ */
{
	zval **progress_ary, **cancel_upload;

	if (zend_symtable_find(Z_ARRVAL_P(PS(http_session_vars)), progress->key.c, progress->key.len+1, (void**)&progress_ary) != SUCCESS) {
		return 0;
	}
	if (Z_TYPE_PP(progress_ary) != IS_ARRAY) {
		return 0;
	}
	if (zend_hash_find(Z_ARRVAL_PP(progress_ary), "cancel_upload", sizeof("cancel_upload"), (void**)&cancel_upload) != SUCCESS) {
		return 0;
	}
	return Z_TYPE_PP(cancel_upload) == IS_BOOL && Z_LVAL_PP(cancel_upload);
} /* }}} */
