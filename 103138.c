PHP_FUNCTION(config_get_hash) /* {{{ */
{
	HashTable *hash = php_ini_get_configuration_hash();

	array_init(return_value);
	zend_hash_apply_with_arguments(hash TSRMLS_CC, (apply_func_args_t) add_config_entry_cb, 1, return_value);
}
/* }}} */
