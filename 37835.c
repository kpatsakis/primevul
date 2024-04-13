find_table_lock(struct net *net, const char *name, int *error,
		struct mutex *mutex)
{
	return find_inlist_lock(&net->xt.tables[NFPROTO_BRIDGE], name,
				"ebtable_", error, mutex);
}
