static void php_session_flush(TSRMLS_D) /* {{{ */
{
	if (PS(session_status) == php_session_active) {
		PS(session_status) = php_session_none;
		php_session_save_current_state(TSRMLS_C);
	}
}
/* }}} */
