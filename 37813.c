ebt_check_match(struct ebt_entry_match *m, struct xt_mtchk_param *par,
		unsigned int *cnt)
{
	const struct ebt_entry *e = par->entryinfo;
	struct xt_match *match;
	size_t left = ((char *)e + e->watchers_offset) - (char *)m;
	int ret;

	if (left < sizeof(struct ebt_entry_match) ||
	    left - sizeof(struct ebt_entry_match) < m->match_size)
		return -EINVAL;

	match = xt_request_find_match(NFPROTO_BRIDGE, m->u.name, 0);
	if (IS_ERR(match))
		return PTR_ERR(match);
	m->u.match = match;

	par->match     = match;
	par->matchinfo = m->data;
	ret = xt_check_match(par, m->match_size,
	      e->ethproto, e->invflags & EBT_IPROTO);
	if (ret < 0) {
		module_put(match->me);
		return ret;
	}

	(*cnt)++;
	return 0;
}
