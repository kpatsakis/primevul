static PHP_INI_MH(OnUpdateRfc1867Freq) /* {{{ */
{
	int tmp;
	tmp = zend_atoi(new_value, new_value_length);
	if(tmp < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "session.upload_progress.freq must be greater than or equal to zero");
		return FAILURE;
	}
	if(new_value_length > 0 && new_value[new_value_length-1] == '%') {
		if(tmp > 100) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "session.upload_progress.freq cannot be over 100%%");
			return FAILURE;
		}
		PS(rfc1867_freq) = -tmp;
	} else {
		PS(rfc1867_freq) = tmp;
	}
	return SUCCESS;
} /* }}} */
