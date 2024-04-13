PHP_FUNCTION(getprotobynumber)
{
	long proto;
	struct protoent *ent;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &proto) == FAILURE) {
		return;
	}

	ent = getprotobynumber(proto);

	if (ent == NULL) {
		RETURN_FALSE;
	}

	RETURN_STRING(ent->p_name, 1);
}
