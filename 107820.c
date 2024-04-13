static inline void sanitize_key(unsigned flags, char *str, size_t len, zval *zv, zend_bool *rfc5987 TSRMLS_DC)
{
	char *eos;

	zval_dtor(zv);
	php_trim(str, len, NULL, 0, zv, 3 TSRMLS_CC);

	if (flags & PHP_HTTP_PARAMS_ESCAPED) {
		sanitize_escaped(zv TSRMLS_CC);
	}
	
	if (!Z_STRLEN_P(zv)) {
		return;
	}

	if (flags & PHP_HTTP_PARAMS_RFC5987) {
		eos = &Z_STRVAL_P(zv)[Z_STRLEN_P(zv)-1];
		if (*eos == '*') {
			*eos = '\0';
			*rfc5987 = 1;
			Z_STRLEN_P(zv) -= 1;
		}
	}

	if (flags & PHP_HTTP_PARAMS_URLENCODED) {
		sanitize_urlencoded(zv TSRMLS_CC);
	}

	if (flags & PHP_HTTP_PARAMS_DIMENSION) {
		sanitize_dimension(zv TSRMLS_CC);
	}
}
