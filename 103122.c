PHP_FUNCTION(set_include_path)
{
	char *new_value;
	int new_value_len;
	char *old_value;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &new_value, &new_value_len) == FAILURE) {
		return;
	}

	old_value = zend_ini_string("include_path", sizeof("include_path"), 0);
	/* copy to return here, because alter might free it! */
	if (old_value) {
		RETVAL_STRING(old_value, 1);
	} else {
		RETVAL_FALSE;
	}

	if (zend_alter_ini_entry_ex("include_path", sizeof("include_path"), new_value, new_value_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME, 0 TSRMLS_CC) == FAILURE) {
		zval_dtor(return_value);
		RETURN_FALSE;
	}
}
