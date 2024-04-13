static PHP_FUNCTION(session_start)
{
	/* skipping check for non-zero args for performance reasons here ?*/
	if (PS(id) && !strlen(PS(id))) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot start session with empty session ID");
		RETURN_FALSE;
	}

	php_session_start(TSRMLS_C);

	if (PS(session_status) != php_session_active) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
