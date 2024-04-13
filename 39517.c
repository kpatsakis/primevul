free_hash_entry(struct ftrace_hash *hash,
		  struct ftrace_func_entry *entry)
{
	hlist_del(&entry->hlist);
	kfree(entry);
	hash->count--;
}
