static PHP_FUNCTION(session_set_cookie_params)
{
	zval **lifetime = NULL;
	char *path = NULL, *domain = NULL;
	int path_len, domain_len, argc = ZEND_NUM_ARGS();
	zend_bool secure = 0, httponly = 0;

	if (!PS(use_cookies) ||
		zend_parse_parameters(argc TSRMLS_CC, "Z|ssbb", &lifetime, &path, &path_len, &domain, &domain_len, &secure, &httponly) == FAILURE) {
		return;
	}

	convert_to_string_ex(lifetime);

	zend_alter_ini_entry("session.cookie_lifetime", sizeof("session.cookie_lifetime"), Z_STRVAL_PP(lifetime), Z_STRLEN_PP(lifetime), PHP_INI_USER, PHP_INI_STAGE_RUNTIME);

	if (path) {
		zend_alter_ini_entry("session.cookie_path", sizeof("session.cookie_path"), path, path_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
	}
	if (domain) {
		zend_alter_ini_entry("session.cookie_domain", sizeof("session.cookie_domain"), domain, domain_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
	}

	if (argc > 3) {
		zend_alter_ini_entry("session.cookie_secure", sizeof("session.cookie_secure"), secure ? "1" : "0", 1, PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
	}
	if (argc > 4) {
		zend_alter_ini_entry("session.cookie_httponly", sizeof("session.cookie_httponly"), httponly ? "1" : "0", 1, PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
	}
}
