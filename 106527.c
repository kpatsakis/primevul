hstore_to_json(PG_FUNCTION_ARGS)
{
	HStore	   *in = PG_GETARG_HS(0);
	int			buflen,
				i;
	int			count = HS_COUNT(in);
	char	   *out,
			   *ptr;
	char	   *base = STRPTR(in);
	HEntry	   *entries = ARRPTR(in);
	StringInfo	src,
				dst;

	if (count == 0)
		PG_RETURN_TEXT_P(cstring_to_text_with_len("{}",2));

	buflen = 3;

	/*
	 * Formula adjusted slightly from the logic in hstore_out. We have to take
	 * account of out treatment of booleans to be a bit more pessimistic about
	 * the length of values.
	 */

	for (i = 0; i < count; i++)
	{
		/* include "" and colon-space and comma-space */
		buflen += 6 + 2 * HS_KEYLEN(entries, i);
		/* include "" only if nonnull */
		buflen += 3 + (HS_VALISNULL(entries, i)
					   ? 1
					   : 2 * HS_VALLEN(entries, i));
	}

	out = ptr = palloc(buflen);

	src = makeStringInfo();
	dst = makeStringInfo();

	*ptr++ = '{';

	for (i = 0; i < count; i++)
	{
		resetStringInfo(src);
		resetStringInfo(dst);
		appendBinaryStringInfo(src, HS_KEY(entries, base, i), HS_KEYLEN(entries, i));
		escape_json(dst, src->data);
		strncpy(ptr, dst->data, dst->len);
		ptr += dst->len;
		*ptr++ = ':';
		*ptr++ = ' ';
		resetStringInfo(dst);
		if (HS_VALISNULL(entries, i))
			appendStringInfoString(dst, "null");
		else
		{
			resetStringInfo(src);
			appendBinaryStringInfo(src, HS_VAL(entries, base, i), HS_VALLEN(entries, i));
			escape_json(dst, src->data);
		}
		strncpy(ptr, dst->data, dst->len);
		ptr += dst->len;

		if (i + 1 != count)
		{
			*ptr++ = ',';
			*ptr++ = ' ';
		}
	}
	*ptr++ = '}';
	*ptr = '\0';

	PG_RETURN_TEXT_P(cstring_to_text(out));
}
