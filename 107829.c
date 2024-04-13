static inline void shift_rfc5988(php_http_buffer_t *buf, char *key_str, size_t key_len, const char *ass, size_t asl, unsigned flags TSRMLS_DC)
{
	char *str;
	size_t len;

	if (buf->used) {
		php_http_buffer_append(buf, ass, asl);
	}

	prepare_key(flags, key_str, key_len, &str, &len TSRMLS_CC);
	php_http_buffer_appends(buf, "<");
	php_http_buffer_append(buf, str, len);
	php_http_buffer_appends(buf, ">");
	efree(str);
}
