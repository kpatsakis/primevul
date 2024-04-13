static void basic_globals_dtor(php_basic_globals *basic_globals_p TSRMLS_DC) /* {{{ */
{
	if (BG(url_adapt_state_ex).tags) {
		zend_hash_destroy(BG(url_adapt_state_ex).tags);
		free(BG(url_adapt_state_ex).tags);
	}
}
/* }}} */
