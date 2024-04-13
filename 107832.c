static void skip_sep(size_t skip, php_http_params_state_t *state, php_http_params_token_t **param, php_http_params_token_t **arg, php_http_params_token_t **val TSRMLS_DC)
{
	size_t sep_len;

	state->input.str += skip;
	state->input.len -= skip;

	while (	(param && (sep_len = check_sep(state, param)))
	||		(arg && (sep_len = check_sep(state, arg)))
	||		(val && (sep_len = check_sep(state, val)))
	) {
		state->input.str += sep_len;
		state->input.len -= sep_len;
	}
}
