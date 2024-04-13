PHP_METHOD(HttpParams, offsetGet)
{
	char *name_str;
	int name_len;
	zval **zparam, *zparams;

	if (SUCCESS != zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name_str, &name_len)) {
		return;
	}

	zparams = php_http_ztyp(IS_ARRAY, zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), 0 TSRMLS_CC));

	if (SUCCESS == zend_symtable_find(Z_ARRVAL_P(zparams), name_str, name_len + 1, (void *) &zparam)) {
		RETVAL_ZVAL(*zparam, 1, 0);
	}

	zval_ptr_dtor(&zparams);
}
