ssize_t ima_parse_add_rule(char *rule)
{
	const char *op = "update_policy";
	char *p;
	struct ima_measure_rule_entry *entry;
	ssize_t result, len;
	int audit_info = 0;

	/* Prevent installed policy from changing */
	if (ima_measure != &measure_default_rules) {
		integrity_audit_msg(AUDIT_INTEGRITY_STATUS, NULL,
				    NULL, op, "already exists",
				    -EACCES, audit_info);
		return -EACCES;
	}

	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		integrity_audit_msg(AUDIT_INTEGRITY_STATUS, NULL,
				    NULL, op, "-ENOMEM", -ENOMEM, audit_info);
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&entry->list);

	p = strsep(&rule, "\n");
	len = strlen(p) + 1;

	if (*p == '#') {
		kfree(entry);
		return len;
	}

	result = ima_parse_rule(p, entry);
	if (result) {
		kfree(entry);
		integrity_audit_msg(AUDIT_INTEGRITY_STATUS, NULL,
				    NULL, op, "invalid policy", result,
				    audit_info);
		return result;
	}

	mutex_lock(&ima_measure_mutex);
	list_add_tail(&entry->list, &measure_policy_rules);
	mutex_unlock(&ima_measure_mutex);

	return len;
}
