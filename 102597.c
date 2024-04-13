static inline char *parse_ip_address_ex(const char *str, size_t str_len, int *portno, int get_err, zend_string **err)
{
	char *colon;
 	char *host = NULL;
 
 #ifdef HAVE_IPV6
 	if (*(str) == '[' && str_len > 1) {
 		/* IPV6 notation to specify raw address with port (i.e. [fe80::1]:80) */
		char *p = memchr(str + 1, ']', str_len - 2), *e = NULL;
 		if (!p || *(p + 1) != ':') {
 			if (get_err) {
 				*err = strpprintf(0, "Failed to parse IPv6 address \"%s\"", str);
 			}
 			return NULL;
 		}
		*portno = strtol(p + 2, &e, 10);
		if (e && *e) {
			if (get_err) {
				*err = strpprintf(0, "Failed to parse address \"%s\"", str);
			}
			return NULL;
		}
 		return estrndup(str + 1, p - str - 1);
 	}
 #endif

 	if (str_len) {
 		colon = memchr(str, ':', str_len - 1);
 	} else {
 		colon = NULL;
 	}

 	if (colon) {
		char *e = NULL;
		*portno = strtol(colon + 1, &e, 10);
		if (!e || !*e) {
			return estrndup(str, colon - str);
 		}
 	}
 
	if (get_err) {
		*err = strpprintf(0, "Failed to parse address \"%s\"", str);
	}
	return NULL;
 }
