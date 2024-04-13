static PHP_RSHUTDOWN_FUNCTION(session) /* {{{ */
{
	int i;

	zend_try {
		php_session_flush(TSRMLS_C);
	} zend_end_try();
	php_rshutdown_session_globals(TSRMLS_C);

	/* this should NOT be done in php_rshutdown_session_globals() */
	for (i = 0; i < 7; i++) {
		if (PS(mod_user_names).names[i] != NULL) {
			zval_ptr_dtor(&PS(mod_user_names).names[i]);
			PS(mod_user_names).names[i] = NULL;
		}
	}

	return SUCCESS;
}
/* }}} */
