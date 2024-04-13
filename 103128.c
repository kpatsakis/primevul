PHP_FUNCTION(ignore_user_abort)
{
	char *arg = NULL;
	int arg_len = 0;
	int old_setting;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &arg, &arg_len) == FAILURE) {
		return;
	}

	old_setting = PG(ignore_user_abort);

	if (arg) {
		zend_alter_ini_entry_ex("ignore_user_abort", sizeof("ignore_user_abort"), arg, arg_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME, 0 TSRMLS_CC);
	}

	RETURN_LONG(old_setting);
}
