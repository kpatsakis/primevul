enum act_return action_http_set_status(struct act_rule *rule, struct proxy *px,
                                       struct session *sess, struct stream *s, int flags)
{
	http_set_status(rule->arg.status.code, rule->arg.status.reason, s);
	return ACT_RET_CONT;
}
