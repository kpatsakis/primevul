void php_http_params_separator_free(php_http_params_token_t **separator)
{
	php_http_params_token_t **sep = separator;
	if (sep) {
		while (*sep) {
			PTR_FREE((*sep)->str);
			efree(*sep);
			++sep;
		}
		efree(separator);
	}
}
