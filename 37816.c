ebt_cleanup_match(struct ebt_entry_match *m, struct net *net, unsigned int *i)
{
	struct xt_mtdtor_param par;

	if (i && (*i)-- == 0)
		return 1;

	par.net       = net;
	par.match     = m->u.match;
	par.matchinfo = m->data;
	par.family    = NFPROTO_BRIDGE;
	if (par.match->destroy != NULL)
		par.match->destroy(&par);
	module_put(par.match->me);
	return 0;
}
