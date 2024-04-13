PHP_FUNCTION(getservbyport)
{
	char *proto;
	int proto_len;
	long port;
	struct servent *serv;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &port, &proto, &proto_len) == FAILURE) {
		return;
	}

	serv = getservbyport(htons((unsigned short) port), proto);

	if (serv == NULL) {
		RETURN_FALSE;
	}

	RETURN_STRING(serv->s_name, 1);
}
