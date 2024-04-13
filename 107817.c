static inline void quote_string(zval *zv, zend_bool force TSRMLS_DC)
{
	int len = Z_STRLEN_P(zv);

	Z_STRVAL_P(zv) = php_addcslashes(Z_STRVAL_P(zv), Z_STRLEN_P(zv), &Z_STRLEN_P(zv), 1,
			ZEND_STRL("\0..\37\173\\\"") TSRMLS_CC);

	if (force || len != Z_STRLEN_P(zv) || strpbrk(Z_STRVAL_P(zv), "()<>@,;:\"[]?={} ")) {
		zval tmp = *zv;
		int len = Z_STRLEN_P(zv) + 2;
		char *str = emalloc(len + 1);

		str[0] = '"';
		memcpy(&str[1], Z_STRVAL_P(zv), Z_STRLEN_P(zv));
		str[len-1] = '"';
		str[len] = '\0';

		zval_dtor(&tmp);
		ZVAL_STRINGL(zv, str, len, 0);
	}
}
