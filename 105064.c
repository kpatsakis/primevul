static PHP_FUNCTION(session_save_path)
{
	char *name = NULL;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &name, &name_len) == FAILURE) {
		return;
	}

	RETVAL_STRING(PS(save_path), 1);

	if (name) {
		if (memchr(name, '\0', name_len) != NULL) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "The save_path cannot contain NULL characters");
			zval_dtor(return_value);
			RETURN_FALSE;
		}
		zend_alter_ini_entry("session.save_path", sizeof("session.save_path"), name, name_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
	}
}
