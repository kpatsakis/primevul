static size_t curl_read(char *data, size_t size, size_t nmemb, void *ctx)
{
	php_curl *ch = (php_curl *)ctx;
	php_curl_read *t = ch->handlers->read;
	int length = 0;

	switch (t->method) {
		case PHP_CURL_DIRECT:
			if (t->fp) {
				length = fread(data, size, nmemb, t->fp);
			}
			break;
		case PHP_CURL_USER: {
			zval argv[3];
			zval retval;
			int  error;
			zend_fcall_info fci;

			ZVAL_RES(&argv[0], ch->res);
			Z_ADDREF(argv[0]);
			ZVAL_RES(&argv[1], t->res);
			Z_ADDREF(argv[1]);
			ZVAL_LONG(&argv[2], (int)size * nmemb);

			fci.size = sizeof(fci);
			fci.function_table = EG(function_table);
			ZVAL_COPY_VALUE(&fci.function_name, &t->func_name);
			fci.object = NULL;
			fci.retval = &retval;
			fci.param_count = 3;
			fci.params = argv;
			fci.no_separation = 0;
			fci.symbol_table = NULL;

			ch->in_callback = 1;
			error = zend_call_function(&fci, &t->fci_cache);
			ch->in_callback = 0;
			if (error == FAILURE) {
				php_error_docref(NULL, E_WARNING, "Cannot call the CURLOPT_READFUNCTION");
#if LIBCURL_VERSION_NUM >= 0x070c01 /* 7.12.1 */
				length = CURL_READFUNC_ABORT;
#endif
			} else if (!Z_ISUNDEF(retval)) {
				if (Z_TYPE(retval) == IS_STRING) {
					length = MIN((int) (size * nmemb), Z_STRLEN(retval));
					memcpy(data, Z_STRVAL(retval), length);
				}
				zval_ptr_dtor(&retval);
			}

			zval_ptr_dtor(&argv[0]);
			zval_ptr_dtor(&argv[1]);
			zval_ptr_dtor(&argv[2]);
			break;
		}
	}

	return length;
}
