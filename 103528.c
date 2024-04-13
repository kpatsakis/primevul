PHP_FUNCTION(curl_pause)
{
	zend_long       bitmask;
	zval       *zid;
	php_curl   *ch;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &zid, &bitmask) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(ch, php_curl *, zid, -1, le_curl_name, le_curl);

	RETURN_LONG(curl_easy_pause(ch->cp, bitmask));
}
