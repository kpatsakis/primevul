PHP_METHOD(HttpParams, __construct)
{
	zval *zcopy, *zparams = NULL, *param_sep = NULL, *arg_sep = NULL, *val_sep = NULL;
	long flags = PHP_HTTP_PARAMS_DEFAULT;
	zend_error_handling zeh;

	php_http_expect(SUCCESS == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z!/z/z/z/l", &zparams, &param_sep, &arg_sep, &val_sep, &flags), invalid_arg, return);

	zend_replace_error_handling(EH_THROW, php_http_exception_runtime_class_entry, &zeh TSRMLS_CC);
	{
		switch (ZEND_NUM_ARGS()) {
			case 5:
				zend_update_property_long(php_http_params_class_entry, getThis(), ZEND_STRL("flags"), flags TSRMLS_CC);
				/* no break */
			case 4:
				zend_update_property(php_http_params_class_entry, getThis(), ZEND_STRL("val_sep"), val_sep TSRMLS_CC);
				/* no break */
			case 3:
				zend_update_property(php_http_params_class_entry, getThis(), ZEND_STRL("arg_sep"), arg_sep TSRMLS_CC);
				/* no break */
			case 2:
				zend_update_property(php_http_params_class_entry, getThis(), ZEND_STRL("param_sep"), param_sep TSRMLS_CC);
				/* no break */
		}

		if (zparams) {
			switch (Z_TYPE_P(zparams)) {
				case IS_OBJECT:
				case IS_ARRAY:
					zcopy = php_http_zsep(1, IS_ARRAY, zparams);
					zend_update_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), zcopy TSRMLS_CC);
					zval_ptr_dtor(&zcopy);
					break;
				default:
					zcopy = php_http_ztyp(IS_STRING, zparams);
					if (Z_STRLEN_P(zcopy)) {
						php_http_params_opts_t opts = {
							{Z_STRVAL_P(zcopy), Z_STRLEN_P(zcopy)},
							php_http_params_separator_init(zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("param_sep"), 0 TSRMLS_CC) TSRMLS_CC),
							php_http_params_separator_init(zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("arg_sep"), 0 TSRMLS_CC) TSRMLS_CC),
							php_http_params_separator_init(zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("val_sep"), 0 TSRMLS_CC) TSRMLS_CC),
							NULL, flags
						};

						MAKE_STD_ZVAL(zparams);
						array_init(zparams);
						php_http_params_parse(Z_ARRVAL_P(zparams), &opts TSRMLS_CC);
						zend_update_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), zparams TSRMLS_CC);
						zval_ptr_dtor(&zparams);

						php_http_params_separator_free(opts.param);
						php_http_params_separator_free(opts.arg);
						php_http_params_separator_free(opts.val);
					}
					zval_ptr_dtor(&zcopy);
					break;
			}
		} else {
			MAKE_STD_ZVAL(zparams);
			array_init(zparams);
			zend_update_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), zparams TSRMLS_CC);
			zval_ptr_dtor(&zparams);
		}
	}
	zend_restore_error_handling(&zeh TSRMLS_CC);
}
