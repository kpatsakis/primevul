static int curl_debug(CURL *cp, curl_infotype type, char *buf, size_t buf_len, void *ctx) /* {{{ */
{
	php_curl *ch = (php_curl *)ctx;

	if (type == CURLINFO_HEADER_OUT) {
		if (ch->header.str) {
			zend_string_release(ch->header.str);
		}
		if (buf_len > 0) {
			ch->header.str = zend_string_init(buf, buf_len, 0);
		}
	}

	return 0;
}
/* }}} */
