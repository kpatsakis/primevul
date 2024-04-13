static void shift_arg(php_http_buffer_t *buf, char *key_str, size_t key_len, zval *zvalue, const char *ass, size_t asl, const char *vss, size_t vsl, unsigned flags TSRMLS_DC)
{
	if (Z_TYPE_P(zvalue) == IS_ARRAY || Z_TYPE_P(zvalue) == IS_OBJECT) {
		HashPosition pos;
		php_http_array_hashkey_t key = php_http_array_hashkey_init(0);
		zval **val;
		zend_bool rfc5987 = !strcmp(key_str, "*rfc5987*");

		if (!rfc5987) {
			shift_key(buf, key_str, key_len, ass, asl, flags TSRMLS_CC);
		}
		FOREACH_KEYVAL(pos, zvalue, key, val) {
			/* did you mean recursion? */
			php_http_array_hashkey_stringify(&key);
			if (rfc5987 && (Z_TYPE_PP(val) == IS_ARRAY || Z_TYPE_PP(val) == IS_OBJECT)) {
				shift_key(buf, key.str, key.len-1, ass, asl, flags TSRMLS_CC);
				shift_rfc5987(buf, *val, vss, vsl, flags TSRMLS_CC);
			} else {
				shift_arg(buf, key.str, key.len-1, *val, ass, asl, vss, vsl, flags TSRMLS_CC);
			}
			php_http_array_hashkey_stringfree(&key);
		}
	} else {
		shift_key(buf, key_str, key_len, ass, asl, flags TSRMLS_CC);

		if (flags & PHP_HTTP_PARAMS_RFC5988) {
			switch (key_len) {
			case lenof("rel"):
			case lenof("title"):
			case lenof("anchor"):
				/* some args must be quoted */
				if (0 <= php_http_select_str(key_str, 3, "rel", "title", "anchor")) {
					shift_rfc5988_val(buf, zvalue, vss, vsl, flags TSRMLS_CC);
					return;
				}
				break;
			}
		}

		shift_val(buf, zvalue, vss, vsl, flags TSRMLS_CC);
	}
}
