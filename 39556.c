ftrace_lookup_ip(struct ftrace_hash *hash, unsigned long ip)
{
	unsigned long key;
	struct ftrace_func_entry *entry;
	struct hlist_head *hhd;

	if (ftrace_hash_empty(hash))
		return NULL;

	if (hash->size_bits > 0)
		key = hash_long(ip, hash->size_bits);
	else
		key = 0;

	hhd = &hash->buckets[key];

	hlist_for_each_entry_rcu(entry, hhd, hlist) {
		if (entry->ip == ip)
			return entry;
	}
	return NULL;
}
