PHP_NAMED_FUNCTION(php_inet_ntop)
{
	char *address;
	int address_len, af = AF_INET;
	char buffer[40];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
		RETURN_FALSE;
	}

#ifdef HAVE_IPV6
	if (address_len == 16) {
		af = AF_INET6;
	} else
#endif
	if (address_len != 4) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid in_addr value");
		RETURN_FALSE;
	}

	if (!inet_ntop(af, address, buffer, sizeof(buffer))) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An unknown error occurred");
		RETURN_FALSE;
	}

	RETURN_STRING(buffer, 1);
}
