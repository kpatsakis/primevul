ebt_check_watcher(struct ebt_entry_watcher *w, struct xt_tgchk_param *par,
		  unsigned int *cnt)
{
	const struct ebt_entry *e = par->entryinfo;
	struct xt_target *watcher;
	size_t left = ((char *)e + e->target_offset) - (char *)w;
	int ret;

	if (left < sizeof(struct ebt_entry_watcher) ||
	   left - sizeof(struct ebt_entry_watcher) < w->watcher_size)
		return -EINVAL;

	watcher = xt_request_find_target(NFPROTO_BRIDGE, w->u.name, 0);
	if (IS_ERR(watcher))
		return PTR_ERR(watcher);
	w->u.watcher = watcher;

	par->target   = watcher;
	par->targinfo = w->data;
	ret = xt_check_target(par, w->watcher_size,
	      e->ethproto, e->invflags & EBT_IPROTO);
	if (ret < 0) {
		module_put(watcher->me);
		return ret;
	}

	(*cnt)++;
	return 0;
}
