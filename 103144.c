PHP_NAMED_FUNCTION(php_inet_pton)
{
	int ret, af = AF_INET;
	char *address;
	int address_len;
	char buffer[17];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address, &address_len) == FAILURE) {
		RETURN_FALSE;
	}

	memset(buffer, 0, sizeof(buffer));

#ifdef HAVE_IPV6
	if (strchr(address, ':')) {
		af = AF_INET6;
	} else
#endif
	if (!strchr(address, '.')) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unrecognized address %s", address);
		RETURN_FALSE;
	}

	ret = inet_pton(af, address, buffer);

	if (ret <= 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unrecognized address %s", address);
		RETURN_FALSE;
	}

	RETURN_STRINGL(buffer, af == AF_INET ? 4 : 16, 1);
}
