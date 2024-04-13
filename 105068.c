static PHP_FUNCTION(session_cache_expire)
{
	zval **expires = NULL;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "|Z", &expires) == FAILURE) {
		return;
	}

	RETVAL_LONG(PS(cache_expire));

	if (argc == 1) {
		convert_to_string_ex(expires);
		zend_alter_ini_entry("session.cache_expire", sizeof("session.cache_expire"), Z_STRVAL_PP(expires), Z_STRLEN_PP(expires), ZEND_INI_USER, ZEND_INI_STAGE_RUNTIME);
	}
}
