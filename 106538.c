hstore_delete_hstore(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	HStore	   *hs2 = PG_GETARG_HS(1);
	HStore	   *out = palloc(VARSIZE(hs));
	int			hs_count = HS_COUNT(hs);
	int			hs2_count = HS_COUNT(hs2);
	char	   *ps,
			   *ps2,
			   *bufd,
			   *pd;
	HEntry	   *es,
			   *es2,
			   *ed;
	int			i,
				j;
	int			outcount = 0;

	SET_VARSIZE(out, VARSIZE(hs));
	HS_SETCOUNT(out, hs_count); /* temporary! */

	ps = STRPTR(hs);
	es = ARRPTR(hs);
	ps2 = STRPTR(hs2);
	es2 = ARRPTR(hs2);
	bufd = pd = STRPTR(out);
	ed = ARRPTR(out);

	if (hs2_count == 0)
	{
		/* return a copy of the input, unchanged */
		memcpy(out, hs, VARSIZE(hs));
		HS_FIXSIZE(out, hs_count);
		HS_SETCOUNT(out, hs_count);
		PG_RETURN_POINTER(out);
	}

	/*
	 * this is in effect a merge between hs and hs2, both of which are already
	 * sorted by (keylen,key); we take keys from hs only; for equal keys, we
	 * take the value from hs unless the values are equal
	 */

	for (i = j = 0; i < hs_count;)
	{
		int			difference;

		if (j >= hs2_count)
			difference = -1;
		else
		{
			int			skeylen = HS_KEYLEN(es, i);
			int			s2keylen = HS_KEYLEN(es2, j);

			if (skeylen == s2keylen)
				difference = memcmp(HS_KEY(es, ps, i),
									HS_KEY(es2, ps2, j),
									skeylen);
			else
				difference = (skeylen > s2keylen) ? 1 : -1;
		}

		if (difference > 0)
			++j;
		else if (difference == 0)
		{
			int			svallen = HS_VALLEN(es, i);
			int			snullval = HS_VALISNULL(es, i);

			if (snullval != HS_VALISNULL(es2, j)
				|| (!snullval
					&& (svallen != HS_VALLEN(es2, j)
			|| memcmp(HS_VAL(es, ps, i), HS_VAL(es2, ps2, j), svallen) != 0)))
			{
				HS_COPYITEM(ed, bufd, pd,
							HS_KEY(es, ps, i), HS_KEYLEN(es, i),
							svallen, snullval);
				++outcount;
			}
			++i, ++j;
		}
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
