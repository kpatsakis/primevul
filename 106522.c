hstore_from_text(PG_FUNCTION_ARGS)
{
	text	   *key;
	text	   *val = NULL;
	Pairs		p;
	HStore	   *out;

	if (PG_ARGISNULL(0))
		PG_RETURN_NULL();

	p.needfree = false;
	key = PG_GETARG_TEXT_PP(0);
	p.key = VARDATA_ANY(key);
	p.keylen = hstoreCheckKeyLen(VARSIZE_ANY_EXHDR(key));

	if (PG_ARGISNULL(1))
	{
		p.vallen = 0;
		p.isnull = true;
	}
	else
	{
		val = PG_GETARG_TEXT_PP(1);
		p.val = VARDATA_ANY(val);
		p.vallen = hstoreCheckValLen(VARSIZE_ANY_EXHDR(val));
		p.isnull = false;
	}

	out = hstorePairs(&p, 1, p.keylen + p.vallen);

	PG_RETURN_POINTER(out);
}
