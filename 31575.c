enum act_return http_action_req_capture(struct act_rule *rule, struct proxy *px,
                                        struct session *sess, struct stream *s, int flags)
{
	struct sample *key;
	struct cap_hdr *h = rule->arg.cap.hdr;
	char **cap = s->req_cap;
	int len;

	key = sample_fetch_as_type(s->be, sess, s, SMP_OPT_DIR_REQ|SMP_OPT_FINAL, rule->arg.cap.expr, SMP_T_STR);
	if (!key)
		return ACT_RET_CONT;

	if (cap[h->index] == NULL)
		cap[h->index] = pool_alloc(h->pool);

	if (cap[h->index] == NULL) /* no more capture memory */
		return ACT_RET_CONT;

	len = key->data.u.str.len;
	if (len > h->len)
		len = h->len;

	memcpy(cap[h->index], key->data.u.str.str, len);
	cap[h->index][len] = 0;
	return ACT_RET_CONT;
}
