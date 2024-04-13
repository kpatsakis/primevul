static PHP_FUNCTION(session_module_name)
{
	char *name = NULL;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &name, &name_len) == FAILURE) {
		return;
	}

	/* Set return_value to current module name */
	if (PS(mod) && PS(mod)->s_name) {
		RETVAL_STRING(safe_estrdup(PS(mod)->s_name), 0);
	} else {
		RETVAL_EMPTY_STRING();
	}

	if (name) {
		if (!_php_find_ps_module(name TSRMLS_CC)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot find named PHP session module (%s)", name);

			zval_dtor(return_value);
			RETURN_FALSE;
		}
		if (PS(mod_data) || PS(mod_user_implemented)) {
			PS(mod)->s_close(&PS(mod_data) TSRMLS_CC);
		}
		PS(mod_data) = NULL;

		zend_alter_ini_entry("session.save_handler", sizeof("session.save_handler"), name, name_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
	}
}
