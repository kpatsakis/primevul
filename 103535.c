static void _php_curl_reset_handlers(php_curl *ch)
{
	if (!Z_ISUNDEF(ch->handlers->write->stream)) {
		zval_ptr_dtor(&ch->handlers->write->stream);
		ZVAL_UNDEF(&ch->handlers->write->stream);
	}
	ch->handlers->write->fp = NULL;
	ch->handlers->write->method = PHP_CURL_STDOUT;

	if (!Z_ISUNDEF(ch->handlers->write_header->stream)) {
		zval_ptr_dtor(&ch->handlers->write_header->stream);
		ZVAL_UNDEF(&ch->handlers->write_header->stream);
	}
	ch->handlers->write_header->fp = NULL;
	ch->handlers->write_header->method = PHP_CURL_IGNORE;

	if (!Z_ISUNDEF(ch->handlers->read->stream)) {
		zval_ptr_dtor(&ch->handlers->read->stream);
		ZVAL_UNDEF(&ch->handlers->read->stream);
	}
	ch->handlers->read->fp = NULL;
	ch->handlers->read->res = NULL;
	ch->handlers->read->method  = PHP_CURL_DIRECT;

	if (!Z_ISUNDEF(ch->handlers->std_err)) {
		zval_ptr_dtor(&ch->handlers->std_err);
		ZVAL_UNDEF(&ch->handlers->std_err);
	}

	if (ch->handlers->progress) {
		zval_ptr_dtor(&ch->handlers->progress->func_name);
		efree(ch->handlers->progress);
		ch->handlers->progress = NULL;
	}

#if LIBCURL_VERSION_NUM >= 0x071500 /* Available since 7.21.0 */
	if (ch->handlers->fnmatch) {
		zval_ptr_dtor(&ch->handlers->fnmatch->func_name);
		efree(ch->handlers->fnmatch);
		ch->handlers->fnmatch = NULL;
	}
#endif

}
