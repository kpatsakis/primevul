PHP_METHOD(HttpParams, toString)
{
	zval **tmp, *zparams, *zpsep, *zasep, *zvsep, *zflags;
	php_http_buffer_t buf;

	zparams = php_http_zsep(1, IS_ARRAY, zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), 0 TSRMLS_CC));
	zflags = php_http_ztyp(IS_LONG, zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("flags"), 0 TSRMLS_CC));

	zpsep = zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("param_sep"), 0 TSRMLS_CC);
	if (Z_TYPE_P(zpsep) == IS_ARRAY && SUCCESS == zend_hash_get_current_data(Z_ARRVAL_P(zpsep), (void *) &tmp)) {
		zpsep = php_http_ztyp(IS_STRING, *tmp);
	} else {
		zpsep = php_http_ztyp(IS_STRING, zpsep);
	}
	zasep = zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("arg_sep"), 0 TSRMLS_CC);
	if (Z_TYPE_P(zasep) == IS_ARRAY && SUCCESS == zend_hash_get_current_data(Z_ARRVAL_P(zasep), (void *) &tmp)) {
		zasep = php_http_ztyp(IS_STRING, *tmp);
	} else {
		zasep = php_http_ztyp(IS_STRING, zasep);
	}
	zvsep = zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("val_sep"), 0 TSRMLS_CC);
	if (Z_TYPE_P(zvsep) == IS_ARRAY && SUCCESS == zend_hash_get_current_data(Z_ARRVAL_P(zvsep), (void *) &tmp)) {
		zvsep = php_http_ztyp(IS_STRING, *tmp);
	} else {
		zvsep = php_http_ztyp(IS_STRING, zvsep);
	}

	php_http_buffer_init(&buf);
	php_http_params_to_string(&buf, Z_ARRVAL_P(zparams), Z_STRVAL_P(zpsep), Z_STRLEN_P(zpsep), Z_STRVAL_P(zasep), Z_STRLEN_P(zasep), Z_STRVAL_P(zvsep), Z_STRLEN_P(zvsep), Z_LVAL_P(zflags) TSRMLS_CC);

	zval_ptr_dtor(&zparams);
	zval_ptr_dtor(&zpsep);
	zval_ptr_dtor(&zasep);
	zval_ptr_dtor(&zvsep);
	zval_ptr_dtor(&zflags);

	RETVAL_PHP_HTTP_BUFFER_VAL(&buf);
}
