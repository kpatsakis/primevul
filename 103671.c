static int php_pgsql_convert_match(const char *str, size_t str_len, const char *regex , int icase)
{
	regex_t re;
	regmatch_t *subs;
	int regopt = REG_EXTENDED;
	int regerr, ret = SUCCESS;
	size_t i;

	/* Check invalid chars for POSIX regex */
	for (i = 0; i < str_len; i++) {
		if (str[i] == '\n' ||
			str[i] == '\r' ||
			str[i] == '\0' ) {
			return FAILURE;
		}
	}

	if (icase) {
		regopt |= REG_ICASE;
	}

	regerr = regcomp(&re, regex, regopt);
	if (regerr) {
		php_error_docref(NULL, E_WARNING, "Cannot compile regex");
		regfree(&re);
		return FAILURE;
	}
	subs = (regmatch_t *)ecalloc(sizeof(regmatch_t), re.re_nsub+1);

	regerr = regexec(&re, str, re.re_nsub+1, subs, 0);
	if (regerr == REG_NOMATCH) {
#ifdef PHP_DEBUG
		php_error_docref(NULL, E_NOTICE, "'%s' does not match with '%s'", str, regex);
#endif
		ret = FAILURE;
	}
	else if (regerr) {
		php_error_docref(NULL, E_WARNING, "Cannot exec regex");
		ret = FAILURE;
	}
	regfree(&re);
	efree(subs);
	return ret;
}
