static int add_config_entry_cb(zval *entry TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
	zval *retval = (zval *)va_arg(args, zval*);
	zval *tmp;

	if (Z_TYPE_P(entry) == IS_STRING) {
		if (hash_key->nKeyLength > 0) {
			add_assoc_stringl_ex(retval, hash_key->arKey, hash_key->nKeyLength, Z_STRVAL_P(entry), Z_STRLEN_P(entry), 1);
		} else {
			add_index_stringl(retval, hash_key->h, Z_STRVAL_P(entry), Z_STRLEN_P(entry), 1);
		}
	} else if (Z_TYPE_P(entry) == IS_ARRAY) {
		MAKE_STD_ZVAL(tmp);
		array_init(tmp);
		zend_hash_apply_with_arguments(Z_ARRVAL_P(entry) TSRMLS_CC, (apply_func_args_t) add_config_entry_cb, 1, tmp);
		add_assoc_zval_ex(retval, hash_key->arKey, hash_key->nKeyLength, tmp);
	}
	return 0;
}
