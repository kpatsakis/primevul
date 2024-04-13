static void php_session_rfc1867_cleanup(php_session_rfc1867_progress *progress TSRMLS_DC) /* {{{ */
{
	php_session_initialize(TSRMLS_C);
	PS(session_status) = php_session_active;
	IF_SESSION_VARS() {
		zend_hash_del(Z_ARRVAL_P(PS(http_session_vars)), progress->key.c, progress->key.len+1);
	}
	php_session_flush(TSRMLS_C);
} /* }}} */
