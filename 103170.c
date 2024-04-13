void user_shutdown_function_dtor(php_shutdown_function_entry *shutdown_function_entry) /* {{{ */
{
	int i;

	for (i = 0; i < shutdown_function_entry->arg_count; i++) {
		zval_ptr_dtor(&shutdown_function_entry->arguments[i]);
	}
	efree(shutdown_function_entry->arguments);
}
/* }}} */
