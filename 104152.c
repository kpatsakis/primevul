static void php_cgi_globals_ctor(php_cgi_globals_struct *php_cgi_globals TSRMLS_DC)
{
	php_cgi_globals->rfc2616_headers = 0;
	php_cgi_globals->nph = 0;
	php_cgi_globals->check_shebang_line = 1;
	php_cgi_globals->force_redirect = 1;
	php_cgi_globals->redirect_status_env = NULL;
	php_cgi_globals->fix_pathinfo = 1;
	php_cgi_globals->discard_path = 0;
	php_cgi_globals->fcgi_logging = 1;
#ifdef PHP_WIN32
	php_cgi_globals->impersonate = 0;
#endif
	zend_hash_init(&php_cgi_globals->user_config_cache, 0, NULL, (dtor_func_t) user_config_cache_entry_dtor, 1);
}
