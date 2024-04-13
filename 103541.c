static int curl_fnmatch(void *ctx, const char *pattern, const char *string)
{
	php_curl *ch = (php_curl *) ctx;
	php_curl_fnmatch *t = ch->handlers->fnmatch;
	int rval = CURL_FNMATCHFUNC_FAIL;
	switch (t->method) {
		case PHP_CURL_USER: {
			zval argv[3];
			zval retval;
			int  error;
			zend_fcall_info fci;

			ZVAL_RES(&argv[0], ch->res);
			Z_ADDREF(argv[0]);
			ZVAL_STRING(&argv[1], pattern);
			ZVAL_STRING(&argv[2], string);

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
				php_error_docref(NULL, E_WARNING, "Cannot call the CURLOPT_FNMATCH_FUNCTION");
			} else if (!Z_ISUNDEF(retval)) {
				if (Z_TYPE(retval) != IS_LONG) {
					convert_to_long_ex(&retval);
				}
				rval = Z_LVAL(retval);
			}
			zval_ptr_dtor(&argv[0]);
			zval_ptr_dtor(&argv[1]);
			zval_ptr_dtor(&argv[2]);
			break;
		}
	}
	return rval;
}
