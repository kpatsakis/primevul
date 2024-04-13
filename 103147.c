PHPAPI int _php_error_log(int opt_err, char *message, char *opt, char *headers TSRMLS_DC) /* {{{ */
{
	return _php_error_log_ex(opt_err, message, (opt_err == 3) ? strlen(message) : 0, opt, headers TSRMLS_CC);
}
/* }}} */
