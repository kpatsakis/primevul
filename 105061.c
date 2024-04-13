static PHP_FUNCTION(session_name)
{
	char *name = NULL;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &name, &name_len) == FAILURE) {
		return;
	}

	RETVAL_STRING(PS(session_name), 1);

	if (name) {
		zend_alter_ini_entry("session.name", sizeof("session.name"), name, name_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
	}
}
