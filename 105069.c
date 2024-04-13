static PHP_FUNCTION(session_encode)
{
	int len;
	char *enc;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	enc = php_session_encode(&len TSRMLS_CC);
	if (enc == NULL) {
		RETURN_FALSE;
	}

	RETVAL_STRINGL(enc, len, 0);
}
