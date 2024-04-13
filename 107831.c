static inline void shift_val(php_http_buffer_t *buf, zval *zvalue, const char *vss, size_t vsl, unsigned flags TSRMLS_DC)
{
	if (Z_TYPE_P(zvalue) != IS_BOOL) {
		zval *tmp = php_http_zsep(1, IS_STRING, zvalue);

		prepare_value(flags, tmp TSRMLS_CC);
		php_http_buffer_append(buf, vss, vsl);
		php_http_buffer_append(buf, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp));

		zval_ptr_dtor(&tmp);
	} else if (!Z_BVAL_P(zvalue)) {
		php_http_buffer_append(buf, vss, vsl);
		php_http_buffer_appends(buf, "0");
	}
}
