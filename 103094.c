PHP_FUNCTION(putenv)
{
	char *setting;
	int setting_len;
	char *p, **env;
	putenv_entry pe;
#ifdef PHP_WIN32
	char *value = NULL;
	int equals = 0;
	int error_code;
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &setting, &setting_len) == FAILURE) {
		return;
	}
    
    if(setting_len == 0 || setting[0] == '=') {
    	php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid parameter syntax");
    	RETURN_FALSE;
    }

	pe.putenv_string = estrndup(setting, setting_len);
	pe.key = estrndup(setting, setting_len);
	if ((p = strchr(pe.key, '='))) {	/* nullify the '=' if there is one */
		*p = '\0';
#ifdef PHP_WIN32
		equals = 1;
#endif
	}

	pe.key_len = strlen(pe.key);
#ifdef PHP_WIN32
	if (equals) {
		if (pe.key_len < setting_len - 1) {
			value = p + 1;
		} else {
			/* empty string*/
			value = p;
		}
	}
#endif

	zend_hash_del(&BG(putenv_ht), pe.key, pe.key_len+1);

	/* find previous value */
	pe.previous_value = NULL;
	for (env = environ; env != NULL && *env != NULL; env++) {
		if (!strncmp(*env, pe.key, pe.key_len) && (*env)[pe.key_len] == '=') {	/* found it */
#if defined(PHP_WIN32)
			/* must copy previous value because MSVCRT's putenv can free the string without notice */
			pe.previous_value = estrdup(*env);
#else
			pe.previous_value = *env;
#endif
			break;
		}
	}

#if HAVE_UNSETENV
	if (!p) { /* no '=' means we want to unset it */
		unsetenv(pe.putenv_string);
	}
	if (!p || putenv(pe.putenv_string) == 0) { /* success */
#else
# ifndef PHP_WIN32
	if (putenv(pe.putenv_string) == 0) { /* success */
# else
	error_code = SetEnvironmentVariable(pe.key, value);
#  if _MSC_VER < 1500
	/* Yet another VC6 bug, unset may return env not found */
	if (error_code != 0 || 
		(error_code == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND)) {
#  else
	if (error_code != 0) { /* success */
#  endif
# endif
#endif
		zend_hash_add(&BG(putenv_ht), pe.key, pe.key_len + 1, (void **) &pe, sizeof(putenv_entry), NULL);
#ifdef HAVE_TZSET
		if (!strncmp(pe.key, "TZ", pe.key_len)) {
			tzset();
		}
#endif
		RETURN_TRUE;
	} else {
		efree(pe.putenv_string);
		efree(pe.key);
		RETURN_FALSE;
	}
}
/* }}} */
