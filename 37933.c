int ima_match_policy(struct inode *inode, enum ima_hooks func, int mask)
{
	struct ima_measure_rule_entry *entry;

	list_for_each_entry(entry, ima_measure, list) {
		bool rc;

		rc = ima_match_rules(entry, inode, func, mask);
		if (rc)
			return entry->action;
	}
	return 0;
}
