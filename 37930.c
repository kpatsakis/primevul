void ima_delete_rules(void)
{
	struct ima_measure_rule_entry *entry, *tmp;

	mutex_lock(&ima_measure_mutex);
	list_for_each_entry_safe(entry, tmp, &measure_policy_rules, list) {
		list_del(&entry->list);
		kfree(entry);
	}
	mutex_unlock(&ima_measure_mutex);
}
