PHP_FUNCTION(curl_init)
{
	php_curl *ch;
	CURL 	 *cp;
	char 	 *url = NULL;
	size_t		  url_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|s", &url, &url_len) == FAILURE) {
		return;
	}

	cp = curl_easy_init();
	if (!cp) {
		php_error_docref(NULL, E_WARNING, "Could not initialize a new cURL handle");
		RETURN_FALSE;
	}

	ch = alloc_curl_handle();

	ch->cp = cp;

	ch->handlers->write->method = PHP_CURL_STDOUT;
	ch->handlers->read->method  = PHP_CURL_DIRECT;
	ch->handlers->write_header->method = PHP_CURL_IGNORE;

	_php_curl_set_default_options(ch);

	if (url) {
		if (php_curl_option_url(ch, url, url_len) == FAILURE) {
			_php_curl_close_ex(ch);
			RETURN_FALSE;
		}
	}

	ZEND_REGISTER_RESOURCE(return_value, ch, le_curl);
	ch->res = Z_RES_P(return_value);
}
