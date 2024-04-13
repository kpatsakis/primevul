static inline void prepare_value(unsigned flags, zval *zv TSRMLS_DC)
{
	if (flags & PHP_HTTP_PARAMS_URLENCODED) {
		prepare_urlencoded(zv TSRMLS_CC);
	}

	if (flags & PHP_HTTP_PARAMS_ESCAPED) {
		prepare_escaped(zv TSRMLS_CC);
	}
}
