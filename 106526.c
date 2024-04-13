hstore_send(PG_FUNCTION_ARGS)
{
	HStore	   *in = PG_GETARG_HS(0);
	int			i;
	int			count = HS_COUNT(in);
	char	   *base = STRPTR(in);
	HEntry	   *entries = ARRPTR(in);
	StringInfoData buf;

	pq_begintypsend(&buf);

	pq_sendint(&buf, count, 4);

	for (i = 0; i < count; i++)
	{
		int32		keylen = HS_KEYLEN(entries, i);

		pq_sendint(&buf, keylen, 4);
		pq_sendtext(&buf, HS_KEY(entries, base, i), keylen);
		if (HS_VALISNULL(entries, i))
		{
			pq_sendint(&buf, -1, 4);
		}
		else
		{
			int32		vallen = HS_VALLEN(entries, i);

			pq_sendint(&buf, vallen, 4);
			pq_sendtext(&buf, HS_VAL(entries, base, i), vallen);
		}
	}

	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}
