ebt_make_names(struct ebt_entry *e, const char *base, char __user *ubase)
{
	int ret;
	char __user *hlp;
	const struct ebt_entry_target *t;

	if (e->bitmask == 0)
		return 0;

	hlp = ubase + (((char *)e + e->target_offset) - base);
	t = (struct ebt_entry_target *)(((char *)e) + e->target_offset);

	ret = EBT_MATCH_ITERATE(e, ebt_make_matchname, base, ubase);
	if (ret != 0)
		return ret;
	ret = EBT_WATCHER_ITERATE(e, ebt_make_watchername, base, ubase);
	if (ret != 0)
		return ret;
	if (copy_to_user(hlp, t->u.target->name, EBT_FUNCTION_MAXNAMELEN))
		return -EFAULT;
	return 0;
}
