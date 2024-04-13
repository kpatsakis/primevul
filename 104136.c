PHP_FUNCTION(apache_child_terminate) /* {{{ */
{
	if (ZEND_NUM_ARGS() > 0) {
		WRONG_PARAM_COUNT;
	}
	if (fcgi_is_fastcgi()) {
		fcgi_terminate();
	}
}
/* }}} */
