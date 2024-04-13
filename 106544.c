hstore_fetchval(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	text	   *key = PG_GETARG_TEXT_PP(1);
	HEntry	   *entries = ARRPTR(hs);
	text	   *out;
	int			idx = hstoreFindKey(hs, NULL,
									VARDATA_ANY(key), VARSIZE_ANY_EXHDR(key));

	if (idx < 0 || HS_VALISNULL(entries, idx))
		PG_RETURN_NULL();

	out = cstring_to_text_with_len(HS_VAL(entries, STRPTR(hs), idx),
								   HS_VALLEN(entries, idx));

	PG_RETURN_TEXT_P(out);
}
