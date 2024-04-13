static inline void shift_rfc5988_val(php_http_buffer_t *buf, zval *zv, const char *vss, size_t vsl, unsigned flags TSRMLS_DC)
{
	zval *tmp = php_http_zsep(1, IS_STRING, zv);

	quote_string(tmp, 1 TSRMLS_CC);
	php_http_buffer_append(buf, vss, vsl);
	php_http_buffer_append(buf, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp));

	zval_ptr_dtor(&tmp);
}
