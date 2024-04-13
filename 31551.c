struct action_kw *action_http_res_custom(const char *kw)
{
	return action_lookup(&http_res_keywords.list, kw);
}
