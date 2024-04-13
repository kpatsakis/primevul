PHP_FUNCTION(curl_escape)
{
	char       *str = NULL, *res = NULL;
	size_t        str_len = 0;
	zval       *zid;
	php_curl   *ch;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rs", &zid, &str, &str_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(ch, php_curl *, zid, -1, le_curl_name, le_curl);

	if ((res = curl_easy_escape(ch->cp, str, str_len))) {
		RETVAL_STRING(res);
		curl_free(res);
	} else {
		RETURN_FALSE;
	}
}
