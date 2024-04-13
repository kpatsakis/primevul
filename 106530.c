hstore_akeys(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	Datum	   *d;
	ArrayType  *a;
	HEntry	   *entries = ARRPTR(hs);
	char	   *base = STRPTR(hs);
	int			count = HS_COUNT(hs);
	int			i;

	if (count == 0)
	{
		a = construct_empty_array(TEXTOID);
		PG_RETURN_POINTER(a);
	}

	d = (Datum *) palloc(sizeof(Datum) * count);

	for (i = 0; i < count; ++i)
	{
		text	   *item = cstring_to_text_with_len(HS_KEY(entries, base, i),
													HS_KEYLEN(entries, i));

		d[i] = PointerGetDatum(item);
	}

	a = construct_array(d, count,
						TEXTOID, -1, false, 'i');

	PG_RETURN_POINTER(a);
}
