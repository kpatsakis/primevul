int suhosin_header_handler(sapi_header_struct *sapi_header, sapi_header_op_enum op, sapi_headers_struct *sapi_headers TSRMLS_DC)
#else
int suhosin_header_handler(sapi_header_struct *sapi_header, sapi_headers_struct *sapi_headers TSRMLS_DC)
#endif
{
	int retval = SAPI_HEADER_ADD, i;
	char *tmp;

#if PHP_VERSION_ID >= 50300
	if (op != SAPI_HEADER_ADD && op != SAPI_HEADER_REPLACE) {
                goto suhosin_skip_header_handling;
	}
#endif
	
	if (sapi_header && sapi_header->header) {
	
		tmp = sapi_header->header;

		for (i=0; i<sapi_header->header_len; i++, tmp++) {
			if (tmp[0] == 0) {
				char *fname = get_active_function_name(TSRMLS_C);

				if (!fname) {
					fname = "unknown";
				}

				suhosin_log(S_MISC, "%s() - wanted to send a HTTP header with an ASCII NUL in it", fname);
				if (!SUHOSIN_G(simulation)) {
					sapi_header->header_len = i;
				}
			}
			if (SUHOSIN_G(allow_multiheader)) {
				continue;
			} else if ((tmp[0] == '\r' && (tmp[1] != '\n' || i == 0)) || 
			   (tmp[0] == '\n' && (i == sapi_header->header_len-1 || i == 0 || (tmp[1] != ' ' && tmp[1] != '\t')))) {
				char *fname = get_active_function_name(TSRMLS_C);

				if (!fname) {
					fname = "unknown";
				}

				suhosin_log(S_MISC, "%s() - wanted to send multiple HTTP headers at once", fname);
				if (!SUHOSIN_G(simulation)) {
					sapi_header->header_len = i;
					tmp[0] = 0;
				}
			}
		}
	}

	/* Handle a potential cookie */

	if (SUHOSIN_G(cookie_encrypt) && (strncasecmp("Set-Cookie:", sapi_header->header, sizeof("Set-Cookie:")-1) == 0)) {

		char *start, *end, *rend, *tmp;
    		char *name, *value;
    		int nlen, vlen, len, tlen;
		char cryptkey[33];

		suhosin_generate_key(SUHOSIN_G(cookie_cryptkey), SUHOSIN_G(cookie_cryptua), SUHOSIN_G(cookie_cryptdocroot), SUHOSIN_G(cookie_cryptraddr), (char *)&cryptkey TSRMLS_CC);
		start = estrndup(sapi_header->header, sapi_header->header_len);
		rend = end = start + sapi_header->header_len;

		tmp = memchr(start, ';', end-start);
		if (tmp != NULL) {
			end = tmp;
		}

		tmp = start + sizeof("Set-Cookie:") - 1;
		while (tmp < end && tmp[0]==' ') {
			tmp++;
		}
		name = tmp;
		nlen = end-name;
		tmp = memchr(name, '=', nlen);
		if (tmp == NULL) {
			value = end;
		} else {
			value = tmp+1;
			nlen = tmp-name;
		}
		vlen = end-value;

		value = suhosin_encrypt_single_cookie(name, nlen, value, vlen, (char *)&cryptkey TSRMLS_CC); 
		vlen = strlen(value);
		
		len = sizeof("Set-Cookie: ")-1 + nlen + 1 + vlen + rend-end;
		tmp = emalloc(len + 1);
		tlen = sprintf(tmp, "Set-Cookie: %.*s=%s", nlen,name, value);
		memcpy(tmp + tlen, end, rend-end);
		tmp[len] = 0;

		efree(sapi_header->header);
		efree(value);
		efree(start);

		sapi_header->header = tmp;
		sapi_header->header_len = len;
	}

suhosin_skip_header_handling:
	/* If existing call the sapi header handler */
	if (orig_header_handler) {
#if PHP_VERSION_ID >= 50300
		retval = orig_header_handler(sapi_header, op, sapi_headers TSRMLS_CC);
#else
		retval = orig_header_handler(sapi_header, sapi_headers TSRMLS_CC);
#endif
	}

	return retval;
}
