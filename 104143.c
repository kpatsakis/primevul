static void add_request_header(char *var, unsigned int var_len, char *val, unsigned int val_len, void *arg TSRMLS_DC) /* {{{ */
{
	zval *return_value = (zval*)arg;
	char *str = NULL;
	char *p;
	ALLOCA_FLAG(use_heap)

	if (var_len > 5 &&
	    var[0] == 'H' &&
	    var[1] == 'T' &&
	    var[2] == 'T' &&
	    var[3] == 'P' &&
	    var[4] == '_') {

		var_len -= 5;
		p = var + 5;
		var = str = do_alloca(var_len + 1, use_heap);
		*str++ = *p++;
		while (*p) {
			if (*p == '_') {
				*str++ = '-';
				p++;
				if (*p) {
					*str++ = *p++;
				}
			} else if (*p >= 'A' && *p <= 'Z') {
				*str++ = (*p++ - 'A' + 'a');
			} else {
				*str++ = *p++;
			}
		}
		*str = 0;
	} else if (var_len == sizeof("CONTENT_TYPE")-1 &&
	           memcmp(var, "CONTENT_TYPE", sizeof("CONTENT_TYPE")-1) == 0) {
		var = "Content-Type";
	} else if (var_len == sizeof("CONTENT_LENGTH")-1 &&
	           memcmp(var, "CONTENT_LENGTH", sizeof("CONTENT_LENGTH")-1) == 0) {
		var = "Content-Length";
	} else {
		return;
	}
	add_assoc_stringl_ex(return_value, var, var_len+1, val, val_len, 1);
	if (str) {
		free_alloca(var, use_heap);
	}
}
/* }}} */
