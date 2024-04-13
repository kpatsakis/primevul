hstore_exists_any(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	ArrayType  *keys = PG_GETARG_ARRAYTYPE_P(1);
	int			nkeys;
	Pairs	   *key_pairs = hstoreArrayToPairs(keys, &nkeys);
	int			i;
	int			lowbound = 0;
	bool		res = false;

	/*
	 * we exploit the fact that the pairs list is already sorted into strictly
	 * increasing order to narrow the hstoreFindKey search; each search can
	 * start one entry past the previous "found" entry, or at the lower bound
	 * of the last search.
	 */
	for (i = 0; i < nkeys; i++)
	{
		int			idx = hstoreFindKey(hs, &lowbound,
									  key_pairs[i].key, key_pairs[i].keylen);

		if (idx >= 0)
		{
			res = true;
			break;
		}
	}

	PG_RETURN_BOOL(res);
}
