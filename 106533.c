hstore_concat(PG_FUNCTION_ARGS)
{
	HStore	   *s1 = PG_GETARG_HS(0);
	HStore	   *s2 = PG_GETARG_HS(1);
	HStore	   *out = palloc(VARSIZE(s1) + VARSIZE(s2));
	char	   *ps1,
			   *ps2,
			   *bufd,
			   *pd;
	HEntry	   *es1,
			   *es2,
			   *ed;
	int			s1idx;
	int			s2idx;
	int			s1count = HS_COUNT(s1);
	int			s2count = HS_COUNT(s2);
	int			outcount = 0;

	SET_VARSIZE(out, VARSIZE(s1) + VARSIZE(s2) - HSHRDSIZE);
	HS_SETCOUNT(out, s1count + s2count);

	if (s1count == 0)
	{
		/* return a copy of the input, unchanged */
		memcpy(out, s2, VARSIZE(s2));
		HS_FIXSIZE(out, s2count);
		HS_SETCOUNT(out, s2count);
		PG_RETURN_POINTER(out);
	}

	if (s2count == 0)
	{
		/* return a copy of the input, unchanged */
		memcpy(out, s1, VARSIZE(s1));
		HS_FIXSIZE(out, s1count);
		HS_SETCOUNT(out, s1count);
		PG_RETURN_POINTER(out);
	}

	ps1 = STRPTR(s1);
	ps2 = STRPTR(s2);
	bufd = pd = STRPTR(out);
	es1 = ARRPTR(s1);
	es2 = ARRPTR(s2);
	ed = ARRPTR(out);

	/*
	 * this is in effect a merge between s1 and s2, both of which are already
	 * sorted by (keylen,key); we take s2 for equal keys
	 */

	for (s1idx = s2idx = 0; s1idx < s1count || s2idx < s2count; ++outcount)
	{
		int			difference;

		if (s1idx >= s1count)
			difference = 1;
		else if (s2idx >= s2count)
			difference = -1;
		else
		{
			int			s1keylen = HS_KEYLEN(es1, s1idx);
			int			s2keylen = HS_KEYLEN(es2, s2idx);

			if (s1keylen == s2keylen)
				difference = memcmp(HS_KEY(es1, ps1, s1idx),
									HS_KEY(es2, ps2, s2idx),
									s1keylen);
			else
				difference = (s1keylen > s2keylen) ? 1 : -1;
		}

		if (difference >= 0)
		{
			HS_COPYITEM(ed, bufd, pd,
						HS_KEY(es2, ps2, s2idx), HS_KEYLEN(es2, s2idx),
						HS_VALLEN(es2, s2idx), HS_VALISNULL(es2, s2idx));
			++s2idx;
			if (difference == 0)
				++s1idx;
		}
		else
		{
			HS_COPYITEM(ed, bufd, pd,
						HS_KEY(es1, ps1, s1idx), HS_KEYLEN(es1, s1idx),
						HS_VALLEN(es1, s1idx), HS_VALISNULL(es1, s1idx));
			++s1idx;
		}
	}

	HS_FINALIZE(out, outcount, bufd, pd);

	PG_RETURN_POINTER(out);
}
