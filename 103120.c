PHP_FUNCTION(ini_set)
{
	char *varname, *new_value;
	int varname_len, new_value_len;
	char *old_value;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &varname, &varname_len, &new_value, &new_value_len) == FAILURE) {
		return;
	}

	old_value = zend_ini_string(varname, varname_len + 1, 0);

	/* copy to return here, because alter might free it! */
	if (old_value) {
		RETVAL_STRING(old_value, 1);
	} else {
		RETVAL_FALSE;
	}

#define _CHECK_PATH(var, var_len, ini) php_ini_check_path(var, var_len, ini, sizeof(ini))
	/* open basedir check */
	if (PG(open_basedir)) {
		if (_CHECK_PATH(varname, varname_len, "error_log") ||
			_CHECK_PATH(varname, varname_len, "java.class.path") ||
			_CHECK_PATH(varname, varname_len, "java.home") ||
			_CHECK_PATH(varname, varname_len, "mail.log") ||
			_CHECK_PATH(varname, varname_len, "java.library.path") ||
			_CHECK_PATH(varname, varname_len, "vpopmail.directory")) {
			if (php_check_open_basedir(new_value TSRMLS_CC)) {
				zval_dtor(return_value);
				RETURN_FALSE;
			}
		}
	}

	if (zend_alter_ini_entry_ex(varname, varname_len + 1, new_value, new_value_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME, 0 TSRMLS_CC) == FAILURE) {
		zval_dtor(return_value);
		RETURN_FALSE;
	}
}
