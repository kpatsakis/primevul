static inline void sanitize_rfc5988(char *str, size_t len, zval *zv TSRMLS_DC)
{
	zval_dtor(zv);
	php_trim(str, len, " ><", 3, zv, 3 TSRMLS_CC);
}
