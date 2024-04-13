hstore_avals(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	Datum	   *d;
	bool	   *nulls;
	ArrayType  *a;
	HEntry	   *entries = ARRPTR(hs);
	char	   *base = STRPTR(hs);
	int			count = HS_COUNT(hs);
	int			lb = 1;
	int			i;

	if (count == 0)
	{
		a = construct_empty_array(TEXTOID);
		PG_RETURN_POINTER(a);
	}

	d = (Datum *) palloc(sizeof(Datum) * count);
	nulls = (bool *) palloc(sizeof(bool) * count);

	for (i = 0; i < count; ++i)
	{
		if (HS_VALISNULL(entries, i))
		{
			d[i] = (Datum) 0;
			nulls[i] = true;
		}
		else
		{
			text	   *item = cstring_to_text_with_len(HS_VAL(entries, base, i),
													  HS_VALLEN(entries, i));

			d[i] = PointerGetDatum(item);
			nulls[i] = false;
		}
	}

	a = construct_md_array(d, nulls, 1, &count, &lb,
						   TEXTOID, -1, false, 'i');

	PG_RETURN_POINTER(a);
}
