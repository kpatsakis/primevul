HashTable *php_http_params_parse(HashTable *params, const php_http_params_opts_t *opts TSRMLS_DC)
{
	php_http_params_state_t state = {{NULL,0}, {NULL,0}, {NULL,0}, {NULL,0}, {NULL,NULL,NULL}, 0, 0};

	state.input.str = opts->input.str;
	state.input.len = opts->input.len;

	if (!params) {
		ALLOC_HASHTABLE(params);
		ZEND_INIT_SYMTABLE(params);
	}

	while (state.input.len) {
		if ((opts->flags & PHP_HTTP_PARAMS_RFC5988) && !state.arg.str) {
			if (*state.input.str == '<') {
				state.quotes = 1;
			} else if (*state.input.str == '>') {
				state.quotes = 0;
			}
		} else if (*state.input.str == '"' && !state.escape) {
			state.quotes = !state.quotes;
		} else {
			state.escape = (*state.input.str == '\\');
		}
		
		if (!state.param.str) {
			/* initialize */
			skip_sep(0, &state, opts->param, opts->arg, opts->val TSRMLS_CC);
			state.param.str = state.input.str;
		} else {
			size_t sep_len;
			/* are we at a param separator? */
			if (0 < (sep_len = check_sep(&state, opts->param))) {
				push_param(params, &state, opts TSRMLS_CC);

				skip_sep(sep_len, &state, opts->param, opts->arg, opts->val TSRMLS_CC);

				/* start off with a new param */
				state.param.str = state.input.str;
				state.param.len = 0;
				state.arg.str = NULL;
				state.arg.len = 0;
				state.val.str = NULL;
				state.val.len = 0;

				continue;

			} else
			/* are we at an arg separator? */
			if (0 < (sep_len = check_sep(&state, opts->arg))) {
				push_param(params, &state, opts TSRMLS_CC);

				skip_sep(sep_len, &state, NULL, opts->arg, opts->val TSRMLS_CC);

				/* continue with a new arg */
				state.arg.str = state.input.str;
				state.arg.len = 0;
				state.val.str = NULL;
				state.val.len = 0;

				continue;

			} else
			/* are we at a val separator? */
			if (0 < (sep_len = check_sep(&state, opts->val))) {
				/* only handle separator if we're not already reading in a val */
				if (!state.val.str) {
					push_param(params, &state, opts TSRMLS_CC);

					skip_sep(sep_len, &state, NULL, NULL, opts->val TSRMLS_CC);

					state.val.str = state.input.str;
					state.val.len = 0;

					continue;
				}
			}
		}
		
		if (state.input.len) {
			++state.input.str;
			--state.input.len;
		}
	}
	/* finalize */
	push_param(params, &state, opts TSRMLS_CC);

	return params;
}
