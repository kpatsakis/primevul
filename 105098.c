PHPAPI void php_add_session_var(char *name, size_t namelen TSRMLS_DC) /* {{{ */
{
	zval **sym_track = NULL;

	IF_SESSION_VARS() {
		zend_hash_find(Z_ARRVAL_P(PS(http_session_vars)), name, namelen + 1, (void *) &sym_track);
	} else {
		return;
	}

	if (sym_track == NULL) {
		zval *empty_var;

		ALLOC_INIT_ZVAL(empty_var);
		ZEND_SET_SYMBOL_WITH_LENGTH(Z_ARRVAL_P(PS(http_session_vars)), name, namelen+1, empty_var, 1, 0);
	}
}
/* }}} */
