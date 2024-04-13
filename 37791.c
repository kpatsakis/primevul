static int compat_calc_entry(const struct ebt_entry *e,
			     const struct ebt_table_info *info,
			     const void *base,
			     struct compat_ebt_replace *newinfo)
{
	const struct ebt_entry_target *t;
	unsigned int entry_offset;
	int off, ret, i;

	if (e->bitmask == 0)
		return 0;

	off = 0;
	entry_offset = (void *)e - base;

	EBT_MATCH_ITERATE(e, compat_calc_match, &off);
	EBT_WATCHER_ITERATE(e, compat_calc_watcher, &off);

	t = (const struct ebt_entry_target *) ((char *) e + e->target_offset);

	off += xt_compat_target_offset(t->u.target);
	off += ebt_compat_entry_padsize();

	newinfo->entries_size -= off;

	ret = xt_compat_add_offset(NFPROTO_BRIDGE, entry_offset, off);
	if (ret)
		return ret;

	for (i = 0; i < NF_BR_NUMHOOKS; i++) {
		const void *hookptr = info->hook_entry[i];
		if (info->hook_entry[i] &&
		    (e < (struct ebt_entry *)(base - hookptr))) {
			newinfo->hook_entry[i] -= off;
			pr_debug("0x%08X -> 0x%08X\n",
					newinfo->hook_entry[i] + off,
					newinfo->hook_entry[i]);
		}
	}

	return 0;
}
