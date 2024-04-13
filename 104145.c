static void cgi_php_import_environment_variables(zval *array_ptr TSRMLS_DC)
{
	if (PG(http_globals)[TRACK_VARS_ENV] &&
		array_ptr != PG(http_globals)[TRACK_VARS_ENV] &&
		Z_TYPE_P(PG(http_globals)[TRACK_VARS_ENV]) == IS_ARRAY &&
		zend_hash_num_elements(Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_ENV])) > 0
	) {
		zval_dtor(array_ptr);
		*array_ptr = *PG(http_globals)[TRACK_VARS_ENV];
		INIT_PZVAL(array_ptr);
		zval_copy_ctor(array_ptr);
		return;
	} else if (PG(http_globals)[TRACK_VARS_SERVER] &&
		array_ptr != PG(http_globals)[TRACK_VARS_SERVER] &&
		Z_TYPE_P(PG(http_globals)[TRACK_VARS_SERVER]) == IS_ARRAY &&
		zend_hash_num_elements(Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_SERVER])) > 0
	) {
		zval_dtor(array_ptr);
		*array_ptr = *PG(http_globals)[TRACK_VARS_SERVER];
		INIT_PZVAL(array_ptr);
		zval_copy_ctor(array_ptr);
		return;
	}

	/* call php's original import as a catch-all */
	php_php_import_environment_variables(array_ptr TSRMLS_CC);

	if (fcgi_is_fastcgi()) {
		fcgi_request *request = (fcgi_request*) SG(server_context);
		fcgi_loadenv(request, cgi_php_load_env_var, array_ptr TSRMLS_CC);
	}
}
