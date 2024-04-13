static inline void prepare_urlencoded(zval *zv TSRMLS_DC)
{
	int len;
	char *str =	php_raw_url_encode(Z_STRVAL_P(zv), Z_STRLEN_P(zv), &len);

	zval_dtor(zv);
	ZVAL_STRINGL(zv, str, len, 0);
}
