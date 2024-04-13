PHP_FUNCTION(curl_close)
{
	zval		*zid;
	php_curl	*ch;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zid) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(ch, php_curl *, zid, -1, le_curl_name, le_curl);

	if (ch->in_callback) {
		php_error_docref(NULL, E_WARNING, "Attempt to close cURL handle from a callback");
		return;
	}

	if (Z_REFCOUNT_P(zid) <= 2) {
		zend_list_close(Z_RES_P(zid));
	}
}
