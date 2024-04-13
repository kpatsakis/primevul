static void shift_param(php_http_buffer_t *buf, char *key_str, size_t key_len, zval *zvalue, const char *pss, size_t psl, const char *ass, size_t asl, const char *vss, size_t vsl, unsigned flags, zend_bool rfc5987 TSRMLS_DC)
{
	if (Z_TYPE_P(zvalue) == IS_ARRAY || Z_TYPE_P(zvalue) == IS_OBJECT) {
		/* treat as arguments, unless we care for dimensions or rfc5987 */
		if (flags & PHP_HTTP_PARAMS_DIMENSION) {
			php_http_buffer_t *keybuf = php_http_buffer_from_string(key_str, key_len);
			prepare_dimension(buf, keybuf, zvalue, pss, psl, vss, vsl, flags TSRMLS_CC);
			php_http_buffer_free(&keybuf);
		} else if (rfc5987) {
			shift_key(buf, key_str, key_len, pss, psl, flags TSRMLS_CC);
			shift_rfc5987(buf, zvalue, vss, vsl, flags TSRMLS_CC);
		} else {
			shift_arg(buf, key_str, key_len, zvalue, ass, asl, vss, vsl, flags TSRMLS_CC);
		}
	} else {
		if (flags & PHP_HTTP_PARAMS_RFC5988) {
			shift_rfc5988(buf, key_str, key_len, pss, psl, flags TSRMLS_CC);
		} else {
			shift_key(buf, key_str, key_len, pss, psl, flags TSRMLS_CC);
		}
		shift_val(buf, zvalue, vss, vsl, flags TSRMLS_CC);
	}
}
