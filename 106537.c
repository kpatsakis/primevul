hstore_delete_array(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	HStore	   *out = palloc(VARSIZE(hs));
	int			hs_count = HS_COUNT(hs);
	char	   *ps,
			   *bufd,
			   *pd;
	HEntry	   *es,
			   *ed;
	int			i,
				j;
	int			outcount = 0;
	ArrayType  *key_array = PG_GETARG_ARRAYTYPE_P(1);
	int			nkeys;
	Pairs	   *key_pairs = hstoreArrayToPairs(key_array, &nkeys);

	SET_VARSIZE(out, VARSIZE(hs));
	HS_SETCOUNT(out, hs_count); /* temporary! */

	ps = STRPTR(hs);
	es = ARRPTR(hs);
	bufd = pd = STRPTR(out);
	ed = ARRPTR(out);

	if (nkeys == 0)
	{
		/* return a copy of the input, unchanged */
		memcpy(out, hs, VARSIZE(hs));
		HS_FIXSIZE(out, hs_count);
		HS_SETCOUNT(out, hs_count);
		PG_RETURN_POINTER(out);
	}

	/*
	 * this is in effect a merge between hs and key_pairs, both of which are
	 * already sorted by (keylen,key); we take keys from hs only
	 */

	for (i = j = 0; i < hs_count;)
	{
		int			difference;

		if (j >= nkeys)
			difference = -1;
		else
		{
			int			skeylen = HS_KEYLEN(es, i);

			if (skeylen == key_pairs[j].keylen)
				difference = memcmp(HS_KEY(es, ps, i),
									key_pairs[j].key,
									key_pairs[j].keylen);
			else
				difference = (skeylen > key_pairs[j].keylen) ? 1 : -1;
		}

		if (difference > 0)
			++j;
		else if (difference == 0)
			++i, ++j;
		else
		{
			HS_COPYITEM(ed, bufd, pd,
						HS_KEY(es, ps, i), HS_KEYLEN(es, i),
						HS_VALLEN(es, i), HS_VALISNULL(es, i));
			++outcount;
			++i;
		}
	}

	HS_FINALIZE(out, outcount, bufd, pd);

	PG_RETURN_POINTER(out);
}
