void free_http_req_rules(struct list *r)
{
	struct act_rule *tr, *pr;

	list_for_each_entry_safe(pr, tr, r, list) {
		LIST_DEL(&pr->list);
		if (pr->action == ACT_HTTP_REQ_AUTH)
			free(pr->arg.auth.realm);

		regex_free(&pr->arg.hdr_add.re);
		free(pr);
	}
}
