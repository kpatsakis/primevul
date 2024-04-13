void free_http_res_rules(struct list *r)
{
	struct http_res_rule *tr, *pr;

	list_for_each_entry_safe(pr, tr, r, list) {
		LIST_DEL(&pr->list);
		regex_free(&pr->arg.hdr_add.re);
		free(pr);
	}
}
