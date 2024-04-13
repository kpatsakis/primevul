static PHP_INI_MH(OnUpdateName) /* {{{ */
{
	/* Numeric session.name won't work at all */
	if ((!new_value_length || is_numeric_string(new_value, new_value_length, NULL, NULL, 0))) {
		int err_type;

		if (stage == ZEND_INI_STAGE_RUNTIME || stage == ZEND_INI_STAGE_ACTIVATE || stage == ZEND_INI_STAGE_STARTUP) {
			err_type = E_WARNING;
		} else {
			err_type = E_ERROR;
		}

		/* Do not output error when restoring ini options. */
		if (stage != ZEND_INI_STAGE_DEACTIVATE) {
			php_error_docref(NULL TSRMLS_CC, err_type, "session.name cannot be a numeric or empty '%s'", new_value);
		}
		return FAILURE;
	}

	OnUpdateStringUnempty(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
	return SUCCESS;
}
/* }}} */
