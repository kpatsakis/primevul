struct http_res_action_kw *action_http_res_custom(const char *kw)
{
	if (!LIST_ISEMPTY(&http_res_keywords.list)) {
		struct http_res_action_kw_list *kw_list;
		int i;

		list_for_each_entry(kw_list, &http_res_keywords.list, list) {
			for (i = 0; kw_list->kw[i].kw != NULL; i++) {
				if (!strcmp(kw, kw_list->kw[i].kw))
					return &kw_list->kw[i];
			}
		}
	}
	return NULL;
}
