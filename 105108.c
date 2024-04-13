static char *php_session_encode(int *newlen TSRMLS_DC) /* {{{ */
{
	char *ret = NULL;

	IF_SESSION_VARS() {
		if (!PS(serializer)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown session.serialize_handler. Failed to encode session object");
			ret = NULL;
		} else if (PS(serializer)->encode(&ret, newlen TSRMLS_CC) == FAILURE) {
			ret = NULL;
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot encode non-existent session");
	}
	return ret;
}
/* }}} */
