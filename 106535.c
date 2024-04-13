hstore_contains(PG_FUNCTION_ARGS)
{
	HStore	   *val = PG_GETARG_HS(0);
	HStore	   *tmpl = PG_GETARG_HS(1);
	bool		res = true;
	HEntry	   *te = ARRPTR(tmpl);
	char	   *tstr = STRPTR(tmpl);
	HEntry	   *ve = ARRPTR(val);
	char	   *vstr = STRPTR(val);
	int			tcount = HS_COUNT(tmpl);
	int			lastidx = 0;
	int			i;

	/*
	 * we exploit the fact that keys in "tmpl" are in strictly increasing
	 * order to narrow the hstoreFindKey search; each search can start one
	 * entry past the previous "found" entry, or at the lower bound of the
	 * search
	 */

	for (i = 0; res && i < tcount; ++i)
	{
		int			idx = hstoreFindKey(val, &lastidx,
									  HS_KEY(te, tstr, i), HS_KEYLEN(te, i));

		if (idx >= 0)
		{
			bool		nullval = HS_VALISNULL(te, i);
			int			vallen = HS_VALLEN(te, i);

			if (nullval != HS_VALISNULL(ve, idx)
				|| (!nullval
					&& (vallen != HS_VALLEN(ve, idx)
			 || memcmp(HS_VAL(te, tstr, i), HS_VAL(ve, vstr, idx), vallen))))
				res = false;
		}
		else
			res = false;
	}

	PG_RETURN_BOOL(res);
}
