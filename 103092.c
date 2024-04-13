PHP_FUNCTION(long2ip)
{
	/* "It's a long but it's not, PHP ints are signed */
	char *ip;
	int ip_len;
	unsigned long n;
	struct in_addr myaddr;
#ifdef HAVE_INET_PTON
	char str[40];
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip, &ip_len) == FAILURE) {
		return;
	}

	n = strtoul(ip, NULL, 0);

	myaddr.s_addr = htonl(n);
#ifdef HAVE_INET_PTON
	if (inet_ntop(AF_INET, &myaddr, str, sizeof(str))) {
		RETURN_STRING(str, 1);
	} else {
		RETURN_FALSE;
	}
#else
	RETURN_STRING(inet_ntoa(myaddr), 1);
#endif
}
