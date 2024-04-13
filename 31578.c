enum act_return http_action_set_req_line(struct act_rule *rule, struct proxy *px,
                                         struct session *sess, struct stream *s, int flags)
{
	struct chunk *replace;
	enum act_return ret = ACT_RET_ERR;

	replace = alloc_trash_chunk();
	if (!replace)
		goto leave;

	/* If we have to create a query string, prepare a '?'. */
	if (rule->arg.http.action == 2)
		replace->str[replace->len++] = '?';
	replace->len += build_logline(s, replace->str + replace->len, replace->size - replace->len,
	                              &rule->arg.http.logfmt);

	http_replace_req_line(rule->arg.http.action, replace->str, replace->len, px, s);

	ret = ACT_RET_CONT;

leave:
	free_trash_chunk(replace);
	return ret;
}
