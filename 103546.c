static size_t curl_progress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	php_curl *ch = (php_curl *)clientp;
	php_curl_progress *t = ch->handlers->progress;
	size_t	rval = 0;

#if PHP_CURL_DEBUG
	fprintf(stderr, "curl_progress() called\n");
	fprintf(stderr, "clientp = %x, dltotal = %f, dlnow = %f, ultotal = %f, ulnow = %f\n", clientp, dltotal, dlnow, ultotal, ulnow);
#endif

	switch (t->method) {
		case PHP_CURL_USER: {
			zval argv[5];
			zval retval;
			int  error;
			zend_fcall_info fci;

			ZVAL_RES(&argv[0], ch->res);
			Z_ADDREF(argv[0]);
			ZVAL_LONG(&argv[1], (zend_long)dltotal);
			ZVAL_LONG(&argv[2], (zend_long)dlnow);
			ZVAL_LONG(&argv[3], (zend_long)ultotal);
			ZVAL_LONG(&argv[4], (zend_long)ulnow);

			fci.size = sizeof(fci);
			fci.function_table = EG(function_table);
			ZVAL_COPY_VALUE(&fci.function_name, &t->func_name);
			fci.object = NULL;
			fci.retval = &retval;
			fci.param_count = 5;
			fci.params = argv;
			fci.no_separation = 0;
			fci.symbol_table = NULL;

			ch->in_callback = 1;
			error = zend_call_function(&fci, &t->fci_cache);
			ch->in_callback = 0;
			if (error == FAILURE) {
				php_error_docref(NULL, E_WARNING, "Cannot call the CURLOPT_PROGRESSFUNCTION");
			} else if (!Z_ISUNDEF(retval)) {
				if (Z_TYPE(retval) != IS_LONG) {
					convert_to_long_ex(&retval);
				}
				if (0 != Z_LVAL(retval)) {
					rval = 1;
				}
			}
			zval_ptr_dtor(&argv[0]);
			zval_ptr_dtor(&argv[1]);
			zval_ptr_dtor(&argv[2]);
			zval_ptr_dtor(&argv[3]);
			zval_ptr_dtor(&argv[4]);
			break;
		}
	}
	return rval;
}
