hstore_to_array_internal(HStore *hs, int ndims)
{
	HEntry	   *entries = ARRPTR(hs);
	char	   *base = STRPTR(hs);
	int			count = HS_COUNT(hs);
	int			out_size[2] = {0, 2};
	int			lb[2] = {1, 1};
	Datum	   *out_datums;
	bool	   *out_nulls;
	int			i;

	Assert(ndims < 3);

	if (count == 0 || ndims == 0)
		return construct_empty_array(TEXTOID);

	out_size[0] = count * 2 / ndims;
	out_datums = palloc(sizeof(Datum) * count * 2);
	out_nulls = palloc(sizeof(bool) * count * 2);

	for (i = 0; i < count; ++i)
	{
		text	   *key = cstring_to_text_with_len(HS_KEY(entries, base, i),
												   HS_KEYLEN(entries, i));

		out_datums[i * 2] = PointerGetDatum(key);
		out_nulls[i * 2] = false;

		if (HS_VALISNULL(entries, i))
		{
			out_datums[i * 2 + 1] = (Datum) 0;
			out_nulls[i * 2 + 1] = true;
		}
		else
		{
			text	   *item = cstring_to_text_with_len(HS_VAL(entries, base, i),
													  HS_VALLEN(entries, i));

			out_datums[i * 2 + 1] = PointerGetDatum(item);
			out_nulls[i * 2 + 1] = false;
		}
	}

	return construct_md_array(out_datums, out_nulls,
							  ndims, out_size, lb,
							  TEXTOID, -1, false, 'i');
}
