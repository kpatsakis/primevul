php_http_params_opts_t *php_http_params_opts_default_get(php_http_params_opts_t *opts)
{
	if (!opts) {
		opts = emalloc(sizeof(*opts));
	}

	memcpy(opts, &def_opts, sizeof(def_opts));

	return opts;
}
