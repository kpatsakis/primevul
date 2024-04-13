hstore_out(PG_FUNCTION_ARGS)
{
	HStore	   *in = PG_GETARG_HS(0);
	int			buflen,
				i;
	int			count = HS_COUNT(in);
	char	   *out,
			   *ptr;
	char	   *base = STRPTR(in);
	HEntry	   *entries = ARRPTR(in);

	if (count == 0)
		PG_RETURN_CSTRING(pstrdup(""));

	buflen = 0;

	/*
	 * this loop overestimates due to pessimistic assumptions about escaping,
	 * so very large hstore values can't be output. this could be fixed, but
	 * many other data types probably have the same issue. This replaced code
	 * that used the original varlena size for calculations, which was wrong
	 * in some subtle ways.
	 */

	for (i = 0; i < count; i++)
	{
		/* include "" and => and comma-space */
		buflen += 6 + 2 * HS_KEYLEN(entries, i);
		/* include "" only if nonnull */
		buflen += 2 + (HS_VALISNULL(entries, i)
					   ? 2
					   : 2 * HS_VALLEN(entries, i));
	}

	out = ptr = palloc(buflen);

	for (i = 0; i < count; i++)
	{
		*ptr++ = '"';
		ptr = cpw(ptr, HS_KEY(entries, base, i), HS_KEYLEN(entries, i));
		*ptr++ = '"';
		*ptr++ = '=';
		*ptr++ = '>';
		if (HS_VALISNULL(entries, i))
		{
			*ptr++ = 'N';
			*ptr++ = 'U';
			*ptr++ = 'L';
			*ptr++ = 'L';
		}
		else
		{
			*ptr++ = '"';
			ptr = cpw(ptr, HS_VAL(entries, base, i), HS_VALLEN(entries, i));
			*ptr++ = '"';
		}

		if (i + 1 != count)
		{
			*ptr++ = ',';
			*ptr++ = ' ';
		}
	}
	*ptr = '\0';

	PG_RETURN_CSTRING(out);
}
