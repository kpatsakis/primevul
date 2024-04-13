PHPAPI int php_get_session_var(char *name, size_t namelen, zval ***state_var TSRMLS_DC) /* {{{ */
{
	int ret = FAILURE;

	IF_SESSION_VARS() {
		ret = zend_hash_find(Z_ARRVAL_P(PS(http_session_vars)), name, namelen + 1, (void **) state_var);
	}
	return ret;
}
/* }}} */
