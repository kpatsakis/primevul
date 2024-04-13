PHPAPI zend_bool remove_user_shutdown_function(char *function_name, size_t function_len TSRMLS_DC) /* {{{ */
{
	if (BG(user_shutdown_function_names)) {
		return zend_hash_del_key_or_index(BG(user_shutdown_function_names), function_name, function_len, 0, HASH_DEL_KEY) != FAILURE;
	}

	return 0;
}
/* }}} */
