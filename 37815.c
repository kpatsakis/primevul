ebt_cleanup_entry(struct ebt_entry *e, struct net *net, unsigned int *cnt)
{
	struct xt_tgdtor_param par;
	struct ebt_entry_target *t;

	if (e->bitmask == 0)
		return 0;
	/* we're done */
	if (cnt && (*cnt)-- == 0)
		return 1;
	EBT_WATCHER_ITERATE(e, ebt_cleanup_watcher, net, NULL);
	EBT_MATCH_ITERATE(e, ebt_cleanup_match, net, NULL);
	t = (struct ebt_entry_target *)(((char *)e) + e->target_offset);

	par.net      = net;
	par.target   = t->u.target;
	par.targinfo = t->data;
	par.family   = NFPROTO_BRIDGE;
	if (par.target->destroy != NULL)
		par.target->destroy(&par);
	module_put(par.target->me);
	return 0;
}
