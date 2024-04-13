static inline void prepare_key(unsigned flags, char *old_key, size_t old_len, char **new_key, size_t *new_len TSRMLS_DC)
{
	zval zv;

	INIT_PZVAL(&zv);
	ZVAL_STRINGL(&zv, old_key, old_len, 1);

	if (flags & PHP_HTTP_PARAMS_URLENCODED) {
		prepare_urlencoded(&zv TSRMLS_CC);
	}

	if (flags & PHP_HTTP_PARAMS_ESCAPED) {
		if (flags & PHP_HTTP_PARAMS_RFC5988) {
			prepare_rfc5988(&zv TSRMLS_CC);
		} else {
			prepare_escaped(&zv TSRMLS_CC);
		}
	}

	*new_key = Z_STRVAL(zv);
	*new_len = Z_STRLEN(zv);
}
