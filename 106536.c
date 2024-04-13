hstore_defined(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	text	   *key = PG_GETARG_TEXT_PP(1);
	HEntry	   *entries = ARRPTR(hs);
	int			idx = hstoreFindKey(hs, NULL,
									VARDATA_ANY(key), VARSIZE_ANY_EXHDR(key));
	bool		res = (idx >= 0 && !HS_VALISNULL(entries, idx));

	PG_RETURN_BOOL(res);
}
