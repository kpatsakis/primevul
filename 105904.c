PHP_FUNCTION(pg_unescape_bytea)
{
	char *from = NULL, *to = NULL, *tmp = NULL;
	size_t to_len;
	int from_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
							  &from, &from_len) == FAILURE) {
		return;
	}

#if HAVE_PQUNESCAPEBYTEA
	tmp = (char *)PQunescapeBytea((unsigned char*)from, &to_len);
	to = estrndup(tmp, to_len);
	PQfreemem(tmp);
#else
	to = (char *)php_pgsql_unescape_bytea((unsigned char*)from, &to_len);
#endif
	if (!to) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING,"Invalid parameter");
		RETURN_FALSE;
	}
	RETVAL_STRINGL(to, to_len, 0);
}
