static struct dentry *__d_instantiate_unique(struct dentry *entry,
					     struct inode *inode)
{
	struct dentry *alias;
	int len = entry->d_name.len;
	const char *name = entry->d_name.name;
	unsigned int hash = entry->d_name.hash;

	if (!inode) {
		__d_instantiate(entry, NULL);
		return NULL;
	}

	hlist_for_each_entry(alias, &inode->i_dentry, d_u.d_alias) {
		/*
		 * Don't need alias->d_lock here, because aliases with
		 * d_parent == entry->d_parent are not subject to name or
		 * parent changes, because the parent inode i_mutex is held.
		 */
		if (alias->d_name.hash != hash)
			continue;
		if (alias->d_parent != entry->d_parent)
			continue;
		if (alias->d_name.len != len)
			continue;
		if (dentry_cmp(alias, name, len))
			continue;
		__dget(alias);
		return alias;
	}

	__d_instantiate(entry, inode);
	return NULL;
}
