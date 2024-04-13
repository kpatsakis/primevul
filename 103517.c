PHP_FUNCTION(curl_setopt)
{
	zval       *zid, *zvalue;
	zend_long        options;
	php_curl   *ch;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rlz", &zid, &options, &zvalue) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(ch, php_curl *, zid, -1, le_curl_name, le_curl);

	if (options <= 0 && options != CURLOPT_SAFE_UPLOAD) {
		php_error_docref(NULL, E_WARNING, "Invalid curl configuration option");
		RETURN_FALSE;
	}

	if (_php_curl_setopt(ch, options, zvalue) == SUCCESS) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
