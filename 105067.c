static PHP_FUNCTION(session_cache_limiter)
{
	char *limiter = NULL;
	int limiter_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &limiter, &limiter_len) == FAILURE) {
		return;
	}

	RETVAL_STRING(PS(cache_limiter), 1);

	if (limiter) {
		zend_alter_ini_entry("session.cache_limiter", sizeof("session.cache_limiter"), limiter, limiter_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME);
	}
}
