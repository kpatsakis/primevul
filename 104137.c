PHP_FUNCTION(apache_request_headers) /* {{{ */
{
	if (ZEND_NUM_ARGS() > 0) {
		WRONG_PARAM_COUNT;
	}
	array_init(return_value);
	if (fcgi_is_fastcgi()) {
		fcgi_request *request = (fcgi_request*) SG(server_context);

		fcgi_loadenv(request, add_request_header, return_value TSRMLS_CC);
	} else {
		char buf[128];
		char **env, *p, *q, *var, *val, *t = buf;
		size_t alloc_size = sizeof(buf);
		unsigned long var_len;

		for (env = environ; env != NULL && *env != NULL; env++) {
			val = strchr(*env, '=');
			if (!val) {				/* malformed entry? */
				continue;
			}
			var_len = val - *env;
			if (var_len >= alloc_size) {
				alloc_size = var_len + 64;
				t = (t == buf ? emalloc(alloc_size): erealloc(t, alloc_size));
			}
			var = *env;
			if (var_len > 5 &&
			    var[0] == 'H' &&
			    var[1] == 'T' &&
			    var[2] == 'T' &&
			    var[3] == 'P' &&
			    var[4] == '_') {

				var_len -= 5;

				if (var_len >= alloc_size) {
					alloc_size = var_len + 64;
					t = (t == buf ? emalloc(alloc_size): erealloc(t, alloc_size));
				}
				p = var + 5;

				var = q = t;
				/* First char keep uppercase */
				*q++ = *p++;
				while (*p) {
					if (*p == '=') {
						/* End of name */
						break;
					} else if (*p == '_') {
						*q++ = '-';
						p++;
						/* First char after - keep uppercase */
						if (*p && *p!='=') {
							*q++ = *p++;
						}
					} else if (*p >= 'A' && *p <= 'Z') {
						/* lowercase */
						*q++ = (*p++ - 'A' + 'a');
					} else {
						*q++ = *p++;
					}
				}
				*q = 0;
			} else if (var_len == sizeof("CONTENT_TYPE")-1 &&
			           memcmp(var, "CONTENT_TYPE", sizeof("CONTENT_TYPE")-1) == 0) {
				var = "Content-Type";
			} else if (var_len == sizeof("CONTENT_LENGTH")-1 &&
			           memcmp(var, "CONTENT_LENGTH", sizeof("CONTENT_LENGTH")-1) == 0) {
				var = "Content-Length";
			} else {
				continue;
			}
			val++;
			add_assoc_string_ex(return_value, var, var_len+1, val, 1);
		}
		if (t != buf && t != NULL) {
			efree(t);
		}
	}
}
/* }}} */
