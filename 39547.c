ftrace_hash_move(struct ftrace_ops *ops, int enable,
		 struct ftrace_hash **dst, struct ftrace_hash *src)
{
	struct ftrace_func_entry *entry;
	struct hlist_node *tn;
	struct hlist_head *hhd;
	struct ftrace_hash *old_hash;
	struct ftrace_hash *new_hash;
	unsigned long key;
	int size = src->count;
	int bits = 0;
	int ret;
	int i;

	/*
	 * Remove the current set, update the hash and add
	 * them back.
	 */
	ftrace_hash_rec_disable(ops, enable);

	/*
	 * If the new source is empty, just free dst and assign it
	 * the empty_hash.
	 */
	if (!src->count) {
		free_ftrace_hash_rcu(*dst);
		rcu_assign_pointer(*dst, EMPTY_HASH);
		/* still need to update the function records */
		ret = 0;
		goto out;
	}

	/*
	 * Make the hash size about 1/2 the # found
	 */
	for (size /= 2; size; size >>= 1)
		bits++;

	/* Don't allocate too much */
	if (bits > FTRACE_HASH_MAX_BITS)
		bits = FTRACE_HASH_MAX_BITS;

	ret = -ENOMEM;
	new_hash = alloc_ftrace_hash(bits);
	if (!new_hash)
		goto out;

	size = 1 << src->size_bits;
	for (i = 0; i < size; i++) {
		hhd = &src->buckets[i];
		hlist_for_each_entry_safe(entry, tn, hhd, hlist) {
			if (bits > 0)
				key = hash_long(entry->ip, bits);
			else
				key = 0;
			remove_hash_entry(src, entry);
			__add_hash_entry(new_hash, entry);
		}
	}

	old_hash = *dst;
	rcu_assign_pointer(*dst, new_hash);
	free_ftrace_hash_rcu(old_hash);

	ret = 0;
 out:
	/*
	 * Enable regardless of ret:
	 *  On success, we enable the new hash.
	 *  On failure, we re-enable the original hash.
	 */
	ftrace_hash_rec_enable(ops, enable);

	return ret;
}
