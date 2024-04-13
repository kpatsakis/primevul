ftrace_find_profiled_func(struct ftrace_profile_stat *stat, unsigned long ip)
{
	struct ftrace_profile *rec;
	struct hlist_head *hhd;
	unsigned long key;

	key = hash_long(ip, ftrace_profile_bits);
	hhd = &stat->hash[key];

	if (hlist_empty(hhd))
		return NULL;

	hlist_for_each_entry_rcu(rec, hhd, node) {
		if (rec->ip == ip)
			return rec;
	}

	return NULL;
}
