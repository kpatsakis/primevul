static inline void sanitize_value(unsigned flags, char *str, size_t len, zval *zv, zend_bool rfc5987 TSRMLS_DC)
{
	char *language = NULL;
	zend_bool latin1 = 0;

	zval_dtor(zv);
	php_trim(str, len, NULL, 0, zv, 3 TSRMLS_CC);

	if (rfc5987) {
		sanitize_rfc5987(zv, &language, &latin1 TSRMLS_CC);
	}

	if (flags & PHP_HTTP_PARAMS_ESCAPED) {
		sanitize_escaped(zv TSRMLS_CC);
	}

	if ((flags & PHP_HTTP_PARAMS_URLENCODED) || (rfc5987 && language)) {
		sanitize_urlencoded(zv TSRMLS_CC);
	}

	if (rfc5987 && language) {
		zval *tmp;

		if (latin1) {
			utf8encode(zv);
		}

		MAKE_STD_ZVAL(tmp);
		ZVAL_COPY_VALUE(tmp, zv);
		array_init(zv);
		add_assoc_zval(zv, language, tmp);
		PTR_FREE(language);
	}
}
