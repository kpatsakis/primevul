hstore_slice_to_array(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	HEntry	   *entries = ARRPTR(hs);
	char	   *ptr = STRPTR(hs);
	ArrayType  *key_array = PG_GETARG_ARRAYTYPE_P(1);
	ArrayType  *aout;
	Datum	   *key_datums;
	bool	   *key_nulls;
	Datum	   *out_datums;
	bool	   *out_nulls;
	int			key_count;
	int			i;

	deconstruct_array(key_array,
					  TEXTOID, -1, false, 'i',
					  &key_datums, &key_nulls, &key_count);

	if (key_count == 0)
	{
		aout = construct_empty_array(TEXTOID);
		PG_RETURN_POINTER(aout);
	}

	out_datums = palloc(sizeof(Datum) * key_count);
	out_nulls = palloc(sizeof(bool) * key_count);

	for (i = 0; i < key_count; ++i)
	{
		text	   *key = (text *) DatumGetPointer(key_datums[i]);
		int			idx;

		if (key_nulls[i])
			idx = -1;
		else
			idx = hstoreFindKey(hs, NULL, VARDATA(key), VARSIZE(key) - VARHDRSZ);

		if (idx < 0 || HS_VALISNULL(entries, idx))
		{
			out_nulls[i] = true;
			out_datums[i] = (Datum) 0;
		}
		else
		{
			out_datums[i] = PointerGetDatum(
						  cstring_to_text_with_len(HS_VAL(entries, ptr, idx),
												   HS_VALLEN(entries, idx)));
			out_nulls[i] = false;
		}
	}

	aout = construct_md_array(out_datums, out_nulls,
							  ARR_NDIM(key_array),
							  ARR_DIMS(key_array),
							  ARR_LBOUND(key_array),
							  TEXTOID, -1, false, 'i');

	PG_RETURN_POINTER(aout);
}
