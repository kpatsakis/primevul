hstore_exists(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	text	   *key = PG_GETARG_TEXT_PP(1);
	int			idx = hstoreFindKey(hs, NULL,
									VARDATA_ANY(key), VARSIZE_ANY_EXHDR(key));

	PG_RETURN_BOOL(idx >= 0);
}
