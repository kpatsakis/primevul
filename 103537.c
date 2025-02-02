void _php_curl_verify_handlers(php_curl *ch, int reporterror) /* {{{ */
{
	php_stream *stream;
	if (!ch || !ch->handlers) {
		return;
	}

	if (!Z_ISUNDEF(ch->handlers->std_err)) {
		stream = zend_fetch_resource(&ch->handlers->std_err, -1, NULL, NULL, 2, php_file_le_stream(), php_file_le_pstream());
		if (stream == NULL) {
			if (reporterror) {
				php_error_docref(NULL, E_WARNING, "CURLOPT_STDERR resource has gone away, resetting to stderr");
			}
			zval_ptr_dtor(&ch->handlers->std_err);
			ZVAL_UNDEF(&ch->handlers->std_err);

			curl_easy_setopt(ch->cp, CURLOPT_STDERR, stderr);
		}
	}
	if (ch->handlers->read && !Z_ISUNDEF(ch->handlers->read->stream)) {
		stream = zend_fetch_resource(&ch->handlers->read->stream, -1, NULL, NULL, 2, php_file_le_stream(), php_file_le_pstream());
		if (stream == NULL) {
			if (reporterror) {
				php_error_docref(NULL, E_WARNING, "CURLOPT_INFILE resource has gone away, resetting to default");
			}
			zval_ptr_dtor(&ch->handlers->read->stream);
			ZVAL_UNDEF(&ch->handlers->read->stream);
			ch->handlers->read->res = NULL;
			ch->handlers->read->fp = 0;

			curl_easy_setopt(ch->cp, CURLOPT_INFILE, (void *) ch);
		}
	}
	if (ch->handlers->write_header && !Z_ISUNDEF(ch->handlers->write_header->stream)) {
		stream = zend_fetch_resource(&ch->handlers->write_header->stream, -1, NULL, NULL, 2, php_file_le_stream(), php_file_le_pstream());
		if (stream == NULL) {
			if (reporterror) {
				php_error_docref(NULL, E_WARNING, "CURLOPT_WRITEHEADER resource has gone away, resetting to default");
			}
			zval_ptr_dtor(&ch->handlers->write_header->stream);
			ZVAL_UNDEF(&ch->handlers->write_header->stream);
			ch->handlers->write_header->fp = 0;

			ch->handlers->write_header->method = PHP_CURL_IGNORE;
			curl_easy_setopt(ch->cp, CURLOPT_WRITEHEADER, (void *) ch);
		}
	}
	if (ch->handlers->write && !Z_ISUNDEF(ch->handlers->write->stream)) {
		stream = zend_fetch_resource(&ch->handlers->write->stream, -1, NULL, NULL, 2, php_file_le_stream(), php_file_le_pstream());
		if (stream == NULL) {
			if (reporterror) {
				php_error_docref(NULL, E_WARNING, "CURLOPT_FILE resource has gone away, resetting to default");
			}
			zval_ptr_dtor(&ch->handlers->write->stream);
			ZVAL_UNDEF(&ch->handlers->write->stream);
			ch->handlers->write->fp = 0;

			ch->handlers->write->method = PHP_CURL_STDOUT;
			curl_easy_setopt(ch->cp, CURLOPT_FILE, (void *) ch);
		}
	}
	return ;
}
/* }}} */
