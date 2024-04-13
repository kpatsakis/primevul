static size_t curl_passwd(void *ctx, char *prompt, char *buf, int buflen)
{
	php_curl *ch = (php_curl *) ctx;
	zval *func = &ch->handlers->passwd;
	zval  argv[3];
	zval  retval;
	int   error;
	int   ret = -1;

	ZVAL_RES(&argv[0], ch->res);
	Z_ADDREF(argv[0]);
	ZVAL_STRING(&argv[1], prompt);
	ZVAL_LONG(&argv[2], buflen);

	error = call_user_function(EG(function_table), NULL, func, &retval, 2, argv);
	if (error == FAILURE) {
		php_error_docref(NULL, E_WARNING, "Could not call the CURLOPT_PASSWDFUNCTION");
	} else if (Z_TYPE(retval) == IS_STRING) {
		if (Z_STRLEN(retval) > buflen) {
			php_error_docref(NULL, E_WARNING, "Returned password is too long for libcurl to handle");
		} else {
			memcpy(buf, Z_STRVAL(retval), Z_STRLEN(retval) + 1);
		}
	} else {
		php_error_docref(NULL, E_WARNING, "User handler '%s' did not return a string", Z_STRVAL_P(func));
	}

	zval_ptr_dtor(&argv[0]);
	zval_ptr_dtor(&argv[1]);
	zval_ptr_dtor(&argv[2]);
	zval_ptr_dtor(&retval);

	return ret;
}
