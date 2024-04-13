static int php_ini_get_option(zend_ini_entry *ini_entry TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key) /* {{{ */
{
	zval *ini_array = va_arg(args, zval *);
	int module_number = va_arg(args, int);
	int details = va_arg(args, int);
	zval *option;

	if (module_number != 0 && ini_entry->module_number != module_number) {
		return 0;
	}

	if (hash_key->nKeyLength == 0 ||
		hash_key->arKey[0] != 0
	) {
		if (details) {
			MAKE_STD_ZVAL(option);
			array_init(option);

			if (ini_entry->orig_value) {
				add_assoc_stringl(option, "global_value", ini_entry->orig_value, ini_entry->orig_value_length, 1);
			} else if (ini_entry->value) {
				add_assoc_stringl(option, "global_value", ini_entry->value, ini_entry->value_length, 1);
			} else {
				add_assoc_null(option, "global_value");
			}

			if (ini_entry->value) {
				add_assoc_stringl(option, "local_value", ini_entry->value, ini_entry->value_length, 1);
			} else {
				add_assoc_null(option, "local_value");
			}

			add_assoc_long(option, "access", ini_entry->modifiable);

			add_assoc_zval_ex(ini_array, ini_entry->name, ini_entry->name_length, option);
		} else {
			if (ini_entry->value) {
				add_assoc_stringl(ini_array, ini_entry->name, ini_entry->value, ini_entry->value_length, 1);
			} else {
				add_assoc_null(ini_array, ini_entry->name);
			}
		}
	}
	return 0;
}
/* }}} */
