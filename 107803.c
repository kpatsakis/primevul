static size_t check_sep(php_http_params_state_t *state, php_http_params_token_t **separators)
{
	php_http_params_token_t **sep = separators;

	if (state->quotes || state->escape) {
		return 0;
	}
	
	if (sep) while (*sep) {
		if (check_str(state->input.str, state->input.len, (*sep)->str, (*sep)->len)) {
			return (*sep)->len;
		}
		++sep;
	}
	return 0;
}
