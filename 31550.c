struct action_kw *action_http_req_custom(const char *kw)
{
	return action_lookup(&http_req_keywords.list, kw);
}
