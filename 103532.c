void _php_curl_cleanup_handle(php_curl *ch)
{
	smart_str_free(&ch->handlers->write->buf);
	if (ch->header.str) {
		zend_string_release(ch->header.str);
		ch->header.str = NULL;
	}

	memset(ch->err.str, 0, CURL_ERROR_SIZE + 1);
	ch->err.no = 0;
}
