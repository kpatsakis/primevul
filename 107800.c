PHP_METHOD(HttpParams, offsetUnset)
{
	char *name_str;
	int name_len;
	zval *zparams;

	if (SUCCESS != zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name_str, &name_len)) {
		return;
	}

	zparams = php_http_zsep(1, IS_ARRAY, zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), 0 TSRMLS_CC));

	zend_symtable_del(Z_ARRVAL_P(zparams), name_str, name_len + 1);
	zend_update_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), zparams TSRMLS_CC);

	zval_ptr_dtor(&zparams);
}
