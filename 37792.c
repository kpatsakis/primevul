static int compat_calc_watcher(struct ebt_entry_watcher *w, int *off)
{
	*off += xt_compat_target_offset(w->u.watcher);
	*off += ebt_compat_entry_padsize();
	return 0;
}
