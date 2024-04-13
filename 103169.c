static int user_shutdown_function_call(php_shutdown_function_entry *shutdown_function_entry TSRMLS_DC) /* {{{ */
{
	zval retval;
	char *function_name;

	if (!zend_is_callable(shutdown_function_entry->arguments[0], 0, &function_name TSRMLS_CC)) {
		php_error(E_WARNING, "(Registered shutdown functions) Unable to call %s() - function does not exist", function_name);
		if (function_name) {
			efree(function_name);
		}
		return 0;
	}
	if (function_name) {
		efree(function_name);
	}

	if (call_user_function(EG(function_table), NULL,
				shutdown_function_entry->arguments[0],
				&retval,
				shutdown_function_entry->arg_count - 1,
				shutdown_function_entry->arguments + 1
				TSRMLS_CC ) == SUCCESS)
	{
		zval_dtor(&retval);
	}
	return 0;
}
/* }}} */
