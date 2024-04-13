PHP_METHOD(HttpParams, offsetSet)
{
	zval *nvalue;
	char *name_str;
	int name_len;
	zval **zparam, *zparams;

	if (SUCCESS != zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &name_str, &name_len, &nvalue)) {
		return;
	}

	zparams = php_http_zsep(1, IS_ARRAY, zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), 0 TSRMLS_CC));

	if (name_len) {
		if (Z_TYPE_P(nvalue) == IS_ARRAY) {
			zval *new_zparam;

			if (SUCCESS == zend_symtable_find(Z_ARRVAL_P(zparams), name_str, name_len + 1, (void *) &zparam)) {
				new_zparam = php_http_zsep(1, IS_ARRAY, *zparam);
				array_join(Z_ARRVAL_P(nvalue), Z_ARRVAL_P(new_zparam), 0, 0);
			} else {
				new_zparam = nvalue;
				Z_ADDREF_P(new_zparam);
			}
			add_assoc_zval_ex(zparams, name_str, name_len + 1, new_zparam);
		} else {
			zval *tmp;

			if (SUCCESS == zend_symtable_find(Z_ARRVAL_P(zparams), name_str, name_len + 1, (void *) &zparam)) {
				tmp = php_http_zsep(1, IS_ARRAY, *zparam);
			} else {
				MAKE_STD_ZVAL(tmp);
				array_init(tmp);
			}

			Z_ADDREF_P(nvalue);
			add_assoc_zval_ex(tmp, ZEND_STRS("value"), nvalue);
			add_assoc_zval_ex(zparams, name_str, name_len + 1, tmp);
		}
	} else {
		zval *tmp = php_http_ztyp(IS_STRING, nvalue), *arr;

		MAKE_STD_ZVAL(arr);
		array_init(arr);
		add_assoc_bool_ex(arr, ZEND_STRS("value"), 1);
		add_assoc_zval_ex(zparams, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp) + 1, arr);
		zval_ptr_dtor(&tmp);
	}

	zend_update_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), zparams TSRMLS_CC);
	zval_ptr_dtor(&zparams);
}
