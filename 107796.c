PHP_METHOD(HttpParams, toArray)
{
	zval *zparams;

	if (SUCCESS != zend_parse_parameters_none()) {
		return;
	}
	zparams = zend_read_property(php_http_params_class_entry, getThis(), ZEND_STRL("params"), 0 TSRMLS_CC);
	RETURN_ZVAL(zparams, 1, 0);
}
