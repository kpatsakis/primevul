static size_t curl_write(char *data, size_t size, size_t nmemb, void *ctx)
{
	php_curl *ch = (php_curl *) ctx;
	php_curl_write *t = ch->handlers->write;
	size_t length = size * nmemb;

#if PHP_CURL_DEBUG
	fprintf(stderr, "curl_write() called\n");
	fprintf(stderr, "data = %s, size = %d, nmemb = %d, ctx = %x\n", data, size, nmemb, ctx);
#endif

	switch (t->method) {
		case PHP_CURL_STDOUT:
			PHPWRITE(data, length);
			break;
		case PHP_CURL_FILE:
			return fwrite(data, size, nmemb, t->fp);
		case PHP_CURL_RETURN:
			if (length > 0) {
				smart_str_appendl(&t->buf, data, (int) length);
			}
			break;
		case PHP_CURL_USER: {
			zval argv[2];
			zval retval;
			int  error;
			zend_fcall_info fci;

			ZVAL_RES(&argv[0], ch->res);
			Z_ADDREF(argv[0]);
			ZVAL_STRINGL(&argv[1], data, length);

			fci.size = sizeof(fci);
			fci.function_table = EG(function_table);
			fci.object = NULL;
			ZVAL_COPY_VALUE(&fci.function_name, &t->func_name);
			fci.retval = &retval;
			fci.param_count = 2;
			fci.params = argv;
			fci.no_separation = 0;
			fci.symbol_table = NULL;

			ch->in_callback = 1;
			error = zend_call_function(&fci, &t->fci_cache);
			ch->in_callback = 0;
			if (error == FAILURE) {
				php_error_docref(NULL, E_WARNING, "Could not call the CURLOPT_WRITEFUNCTION");
				length = -1;
			} else if (!Z_ISUNDEF(retval)) {
				if (Z_TYPE(retval) != IS_LONG) {
					convert_to_long_ex(&retval);
				}
				length = Z_LVAL(retval);
			}

			zval_ptr_dtor(&argv[0]);
			zval_ptr_dtor(&argv[1]);
			break;
		}
	}

	return length;
}
