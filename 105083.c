static PHP_INI_MH(OnUpdateHashFunc) /* {{{ */
{
	long val;
	char *endptr = NULL;

#if defined(HAVE_HASH_EXT) && !defined(COMPILE_DL_HASH)
	PS(hash_ops) = NULL;
#endif

	val = strtol(new_value, &endptr, 10);
	if (endptr && (*endptr == '\0')) {
		/* Numeric value */
		PS(hash_func) = val ? 1 : 0;

		return SUCCESS;
	}

	if (new_value_length == (sizeof("md5") - 1) &&
		strncasecmp(new_value, "md5", sizeof("md5") - 1) == 0) {
		PS(hash_func) = PS_HASH_FUNC_MD5;

		return SUCCESS;
	}

	if (new_value_length == (sizeof("sha1") - 1) &&
		strncasecmp(new_value, "sha1", sizeof("sha1") - 1) == 0) {
		PS(hash_func) = PS_HASH_FUNC_SHA1;

		return SUCCESS;
	}

#if defined(HAVE_HASH_EXT) && !defined(COMPILE_DL_HASH) /* {{{ */
{
	php_hash_ops *ops = (php_hash_ops*)php_hash_fetch_ops(new_value, new_value_length);

	if (ops) {
		PS(hash_func) = PS_HASH_FUNC_OTHER;
		PS(hash_ops) = ops;

		return SUCCESS;
	}
}
#endif /* HAVE_HASH_EXT }}} */

	return FAILURE;
}
/* }}} */
