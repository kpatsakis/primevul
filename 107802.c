PHP_MINIT_FUNCTION(http_params)
{
	zend_class_entry ce = {0};

	INIT_NS_CLASS_ENTRY(ce, "http", "Params", php_http_params_methods);
	php_http_params_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	php_http_params_class_entry->create_object = php_http_params_object_new;
	zend_class_implements(php_http_params_class_entry TSRMLS_CC, 1, zend_ce_arrayaccess);

	zend_declare_class_constant_stringl(php_http_params_class_entry, ZEND_STRL("DEF_PARAM_SEP"), ZEND_STRL(",") TSRMLS_CC);
	zend_declare_class_constant_stringl(php_http_params_class_entry, ZEND_STRL("DEF_ARG_SEP"), ZEND_STRL(";") TSRMLS_CC);
	zend_declare_class_constant_stringl(php_http_params_class_entry, ZEND_STRL("DEF_VAL_SEP"), ZEND_STRL("=") TSRMLS_CC);
	zend_declare_class_constant_stringl(php_http_params_class_entry, ZEND_STRL("COOKIE_PARAM_SEP"), ZEND_STRL("") TSRMLS_CC);

	zend_declare_class_constant_long(php_http_params_class_entry, ZEND_STRL("PARSE_RAW"), PHP_HTTP_PARAMS_RAW TSRMLS_CC);
	zend_declare_class_constant_long(php_http_params_class_entry, ZEND_STRL("PARSE_ESCAPED"), PHP_HTTP_PARAMS_ESCAPED TSRMLS_CC);
	zend_declare_class_constant_long(php_http_params_class_entry, ZEND_STRL("PARSE_URLENCODED"), PHP_HTTP_PARAMS_URLENCODED TSRMLS_CC);
	zend_declare_class_constant_long(php_http_params_class_entry, ZEND_STRL("PARSE_DIMENSION"), PHP_HTTP_PARAMS_DIMENSION TSRMLS_CC);
	zend_declare_class_constant_long(php_http_params_class_entry, ZEND_STRL("PARSE_RFC5987"), PHP_HTTP_PARAMS_RFC5987 TSRMLS_CC);
	zend_declare_class_constant_long(php_http_params_class_entry, ZEND_STRL("PARSE_RFC5988"), PHP_HTTP_PARAMS_RFC5988 TSRMLS_CC);
	zend_declare_class_constant_long(php_http_params_class_entry, ZEND_STRL("PARSE_DEFAULT"), PHP_HTTP_PARAMS_DEFAULT TSRMLS_CC);
	zend_declare_class_constant_long(php_http_params_class_entry, ZEND_STRL("PARSE_QUERY"), PHP_HTTP_PARAMS_QUERY TSRMLS_CC);

	zend_declare_property_null(php_http_params_class_entry, ZEND_STRL("params"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_stringl(php_http_params_class_entry, ZEND_STRL("param_sep"), ZEND_STRL(","), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_stringl(php_http_params_class_entry, ZEND_STRL("arg_sep"), ZEND_STRL(";"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_stringl(php_http_params_class_entry, ZEND_STRL("val_sep"), ZEND_STRL("="), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_long(php_http_params_class_entry, ZEND_STRL("flags"), PHP_HTTP_PARAMS_DEFAULT, ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;
}
