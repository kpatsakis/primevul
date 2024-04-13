static PHP_FUNCTION(session_unset)
{
	if (PS(session_status) == php_session_none) {
		RETURN_FALSE;
	}

	IF_SESSION_VARS() {
		HashTable *ht_sess_var;

		SEPARATE_ZVAL_IF_NOT_REF(&PS(http_session_vars));
		ht_sess_var = Z_ARRVAL_P(PS(http_session_vars));

		/* Clean $_SESSION. */
		zend_hash_clean(ht_sess_var);
	}
}
