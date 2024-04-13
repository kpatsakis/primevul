__unregister_ftrace_function_probe(char *glob, struct ftrace_probe_ops *ops,
				  void *data, int flags)
{
	struct ftrace_func_probe *entry;
	struct hlist_node *tmp;
	char str[KSYM_SYMBOL_LEN];
	int type = MATCH_FULL;
	int i, len = 0;
	char *search;

	if (glob && (strcmp(glob, "*") == 0 || !strlen(glob)))
		glob = NULL;
	else if (glob) {
		int not;

		type = filter_parse_regex(glob, strlen(glob), &search, &not);
		len = strlen(search);

		/* we do not support '!' for function probes */
		if (WARN_ON(not))
			return;
	}

	mutex_lock(&ftrace_lock);
	for (i = 0; i < FTRACE_FUNC_HASHSIZE; i++) {
		struct hlist_head *hhd = &ftrace_func_hash[i];

		hlist_for_each_entry_safe(entry, tmp, hhd, node) {

			/* break up if statements for readability */
			if ((flags & PROBE_TEST_FUNC) && entry->ops != ops)
				continue;

			if ((flags & PROBE_TEST_DATA) && entry->data != data)
				continue;

			/* do this last, since it is the most expensive */
			if (glob) {
				kallsyms_lookup(entry->ip, NULL, NULL,
						NULL, str);
				if (!ftrace_match(str, glob, len, type))
					continue;
			}

			hlist_del_rcu(&entry->node);
			call_rcu_sched(&entry->rcu, ftrace_free_entry_rcu);
		}
	}
	__disable_ftrace_function_probe();
	mutex_unlock(&ftrace_lock);
}
