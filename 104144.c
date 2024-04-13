static void add_response_header(sapi_header_struct *h, zval *return_value TSRMLS_DC) /* {{{ */
{
	char *s, *p;
	int  len;
	ALLOCA_FLAG(use_heap)

	if (h->header_len > 0) {
		p = strchr(h->header, ':');
		len = p - h->header;
		if (p && (len > 0)) {
			while (len > 0 && (h->header[len-1] == ' ' || h->header[len-1] == '\t')) {
				len--;
			}
			if (len) {
				s = do_alloca(len + 1, use_heap);
				memcpy(s, h->header, len);
				s[len] = 0;
				do {
					p++;
				} while (*p == ' ' || *p == '\t');
				add_assoc_stringl_ex(return_value, s, len+1, p, h->header_len - (p - h->header), 1);
				free_alloca(s, use_heap);
			}
		}
	}
}
/* }}} */
