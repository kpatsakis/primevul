ebt_cleanup_watcher(struct ebt_entry_watcher *w, struct net *net, unsigned int *i)
{
	struct xt_tgdtor_param par;

	if (i && (*i)-- == 0)
		return 1;

	par.net      = net;
	par.target   = w->u.watcher;
	par.targinfo = w->data;
	par.family   = NFPROTO_BRIDGE;
	if (par.target->destroy != NULL)
		par.target->destroy(&par);
	module_put(par.target->me);
	return 0;
}
