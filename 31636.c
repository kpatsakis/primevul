enum act_parse_ret parse_http_action_reject(const char **args, int *orig_arg, struct proxy *px,
                                            struct act_rule *rule, char **err)
{
	rule->action = ACT_CUSTOM;
	rule->action_ptr = http_action_reject;
	return ACT_RET_PRS_OK;
}
