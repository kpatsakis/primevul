static PHP_FUNCTION(session_id)
{
	char *name = NULL;
	int name_len, argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "|s", &name, &name_len) == FAILURE) {
		return;
	}

	if (PS(id)) {
		RETVAL_STRING(PS(id), 1);
	} else {
		RETVAL_EMPTY_STRING();
	}

	if (name) {
		if (PS(id)) {
			efree(PS(id));
		}
		PS(id) = estrndup(name, name_len);
	}
}
