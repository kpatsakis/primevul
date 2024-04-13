static int compat_copy_everything_to_user(struct ebt_table *t,
					  void __user *user, int *len, int cmd)
{
	struct compat_ebt_replace repl, tmp;
	struct ebt_counter *oldcounters;
	struct ebt_table_info tinfo;
	int ret;
	void __user *pos;

	memset(&tinfo, 0, sizeof(tinfo));

	if (cmd == EBT_SO_GET_ENTRIES) {
		tinfo.entries_size = t->private->entries_size;
		tinfo.nentries = t->private->nentries;
		tinfo.entries = t->private->entries;
		oldcounters = t->private->counters;
	} else {
		tinfo.entries_size = t->table->entries_size;
		tinfo.nentries = t->table->nentries;
		tinfo.entries = t->table->entries;
		oldcounters = t->table->counters;
	}

	if (copy_from_user(&tmp, user, sizeof(tmp)))
		return -EFAULT;

	if (tmp.nentries != tinfo.nentries ||
	   (tmp.num_counters && tmp.num_counters != tinfo.nentries))
		return -EINVAL;

	memcpy(&repl, &tmp, sizeof(repl));
	if (cmd == EBT_SO_GET_ENTRIES)
		ret = compat_table_info(t->private, &repl);
	else
		ret = compat_table_info(&tinfo, &repl);
	if (ret)
		return ret;

	if (*len != sizeof(tmp) + repl.entries_size +
	   (tmp.num_counters? tinfo.nentries * sizeof(struct ebt_counter): 0)) {
		pr_err("wrong size: *len %d, entries_size %u, replsz %d\n",
				*len, tinfo.entries_size, repl.entries_size);
		return -EINVAL;
	}

	/* userspace might not need the counters */
	ret = copy_counters_to_user(t, oldcounters, compat_ptr(tmp.counters),
					tmp.num_counters, tinfo.nentries);
	if (ret)
		return ret;

	pos = compat_ptr(tmp.entries);
	return EBT_ENTRY_ITERATE(tinfo.entries, tinfo.entries_size,
			compat_copy_entry_to_user, &pos, &tmp.entries_size);
}
